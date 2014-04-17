/*
 * Copyright (C) 2007 Apple Inc. All rights reserved.
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

#include "config.h"
#if ENABLE(DATABASE)
#include "JSSQLStatementErrorCallback.h"

#include "Frame.h"
#include "JSSQLError.h"
#include "JSSQLTransaction.h"
#include "ScriptExecutionContext.h"
#include <runtime/JSLock.h>

namespace WebCore {

using namespace JSC;

bool JSSQLStatementErrorCallback::handleEvent(ScriptExecutionContext* context, SQLTransaction* transaction, SQLError* error)
{
    ASSERT(m_data);
    ASSERT(context);

    RefPtr<JSSQLStatementErrorCallback> protect(this);

    JSC::JSLock lock(SilenceAssertionsOnly);
    JSDOMGlobalObject* globalObject = toJSDOMGlobalObject(context, m_isolatedWorld.get());
    if (!globalObject)
        return true; // if we cannot invoke the callback, roll back the transaction

    ExecState* exec = globalObject->globalExec();
    MarkedArgumentBuffer args;
    args.append(toJS(exec, deprecatedGlobalObjectForPrototype(exec), transaction));
    args.append(toJS(exec, deprecatedGlobalObjectForPrototype(exec), error));

    bool raisedException = false;
    JSValue result = m_data->invokeCallback(args, &raisedException);
    if (raisedException) {
        // The spec says:
        // "If the error callback returns false, then move on to the next statement..."
        // "Otherwise, the error callback did not return false, or there was no error callback"
        // Therefore an exception and returning true are the same thing - so, return true on an exception
        return true;
    }
    return result.toBoolean(exec);
}

}

#endif // ENABLE(DATABASE)
