/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Adobe Systems Incorporated.  All rights reserved. 
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

#if ENABLE(APOLLO_PROFILER)

#include "SamplerContext.h"
#include "SamplerApollo.h"

namespace JSC {

SamplerContext::SamplerContext(const char* name)
    : m_name(name)
{
}

SamplerContextPool SamplerContextPool::builtins;

SamplerContextPool::SamplerContextPool()
    : cssParse("[CSS Parser]")
    , cssStyle("[CSS Update]")
    , htmlLayout("[HTML Layout]")
    , htmlPaint("[HTML Paint]")
    , jsParse("[JS Parser]")
    , htmlParse("[HTML Parser]")
    , xmlParse("[XML Parser]")
{
}

void SamplerContextPool::registerPool(SamplerApollo* sampler)
{
#if ENABLE(CONTEXT_SAMPLING)
    sampler->registerNode(adoptRef(new SamplerDataNode<SamplerContext>(&builtins.cssParse)));
    sampler->registerNode(adoptRef(new SamplerDataNode<SamplerContext>(&builtins.cssStyle)));
    sampler->registerNode(adoptRef(new SamplerDataNode<SamplerContext>(&builtins.htmlLayout)));
    sampler->registerNode(adoptRef(new SamplerDataNode<SamplerContext>(&builtins.htmlPaint)));
    sampler->registerNode(adoptRef(new SamplerDataNode<SamplerContext>(&builtins.jsParse)));
    sampler->registerNode(adoptRef(new SamplerDataNode<SamplerContext>(&builtins.htmlParse)));
    sampler->registerNode(adoptRef(new SamplerDataNode<SamplerContext>(&builtins.xmlParse)));
#else
    (void)sampler;
#endif
}

void SamplerContextPool::unregisterPool(SamplerApollo* sampler)
{
#if ENABLE(CONTEXT_SAMPLING)
	sampler->unregisterNodeHelper(&builtins.cssParse);
	sampler->unregisterNodeHelper(&builtins.cssStyle);
	sampler->unregisterNodeHelper(&builtins.htmlLayout);
	sampler->unregisterNodeHelper(&builtins.htmlPaint);
	sampler->unregisterNodeHelper(&builtins.jsParse);
	sampler->unregisterNodeHelper(&builtins.htmlParse);
    sampler->unregisterNodeHelper(&builtins.xmlParse);
#else
    (void)sampler;
#endif
}

EnterSamplerContext::EnterSamplerContext(SamplerContext* context)
    : m_samplerContext(context)
{
#if ENABLE(CONTEXT_SAMPLING)
    if( (m_sampler = *(SamplerApollo::enabledSamplerReference())) )
        m_sampler->willExecute(context);
#endif
}

EnterSamplerContext::~EnterSamplerContext()
{
#if ENABLE(CONTEXT_SAMPLING)
    // send the didExecute only if the sampler is still active
    if(m_sampler && (m_sampler == *(SamplerApollo::enabledSamplerReference())))
        m_sampler->didExecute(m_samplerContext);
#endif
}

} // namespace JSC

#endif // PLATFORM(APOLLO)
