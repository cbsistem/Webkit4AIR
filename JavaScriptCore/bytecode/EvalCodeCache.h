/*
 * Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef EvalCodeCache_h
#define EvalCodeCache_h

#include "Executable.h"
#include "JSGlobalObject.h"
#include "Nodes.h"
#include "Parser.h"
#include "SourceCode.h"
#include "UString.h"
#include <wtf/HashMap.h>
#include <wtf/RefPtr.h>
#include <wtf/text/StringHash.h>

namespace JSC {

    class EvalCodeCache {
    public:
        PassRefPtr<EvalExecutable> get(ExecState* exec, const UString& evalSource, ScopeChainNode* scopeChain, JSValue& exceptionValue)
        {
            RefPtr<EvalExecutable> evalExecutable;

#if PLATFORM(APOLLO)
            // After loading in application sandbox (currentEvalLevel == EvalConstantExpr), eval cache will not be used, 
            // thus all the eval nodes will be fully parsed and the isSafeForEval method will work correctly.
            if (evalSource.size() < maxCacheableSourceLength && (*scopeChain->begin())->isVariableObject() && (scopeChain->globalObject->currentEvalLevel() != EvalAny)) 
#else
            if (evalSource.size() < maxCacheableSourceLength && (*scopeChain->begin())->isVariableObject())
#endif
                evalExecutable = m_cacheMap.get(evalSource.rep());

            if (!evalExecutable) {
                evalExecutable = EvalExecutable::create(exec, makeSource(evalSource));
                exceptionValue = evalExecutable->compile(exec, scopeChain);
                if (exceptionValue)
                    return 0;
#if PLATFORM(APOLLO)
                    // After loading, no eval node will be added to eval cache.
                if (evalSource.size() < maxCacheableSourceLength && (*scopeChain->begin())->isVariableObject() && m_cacheMap.size() < maxCacheEntries && (scopeChain->globalObject->currentEvalLevel() != EvalAny))
#else
                if (evalSource.size() < maxCacheableSourceLength && (*scopeChain->begin())->isVariableObject() && m_cacheMap.size() < maxCacheEntries)
#endif
                    m_cacheMap.set(evalSource.rep(), evalExecutable);
            }

            return evalExecutable.release();
        }

        bool isEmpty() const { return m_cacheMap.isEmpty(); }

    private:
        static const unsigned maxCacheableSourceLength = 256;
        static const int maxCacheEntries = 64;

        typedef HashMap<RefPtr<UString::Rep>, RefPtr<EvalExecutable> > EvalCacheMap;
        EvalCacheMap m_cacheMap;
    };

} // namespace JSC

#endif // EvalCodeCache_h
