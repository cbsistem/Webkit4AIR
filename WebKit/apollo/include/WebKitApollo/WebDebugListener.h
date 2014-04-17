/*
 * Copyright (C) 2007 Adobe Systems Incorporated.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Adobe Systems Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ADOBE SYSTEMS INCORPORATED "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ADOBE SYSTEMS INCORPORATED
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebDebugListener_h
#define WebDebugListener_h


#include <WebKitApollo/WebKit.h>

struct WebDebugListener;

typedef WEBKIT_APOLLO_PROTO1 void ( WEBKIT_APOLLO_PROTO2 *WebDebugListenerSourceParsed )( struct WebDebugListener* pWebDebugListener, intptr_t sourceId, WebString * sourceURL, WebString * source, int startingLineNumber, int errorLine, WebString *errorMsg );
typedef WEBKIT_APOLLO_PROTO1 void ( WEBKIT_APOLLO_PROTO2 *WebDebugListenerException )( struct WebDebugListener* pWebDebugListener, struct WebDebugState *pWebDebugState, intptr_t sourceId, int lineno, bool isCaughtExeption );
typedef WEBKIT_APOLLO_PROTO1 void ( WEBKIT_APOLLO_PROTO2 *WebDebugListenerAtStatement )( struct WebDebugListener* pWebDebugListener, struct WebDebugState *pWebDebugState, intptr_t sourceId, int lineno );
typedef WEBKIT_APOLLO_PROTO1 void ( WEBKIT_APOLLO_PROTO2 *WebDebugListenerCallEvent )( struct WebDebugListener* pWebDebugListener, struct WebDebugState *pWebDebugState, intptr_t sourceId, int lineno );
typedef WEBKIT_APOLLO_PROTO1 void ( WEBKIT_APOLLO_PROTO2 *WebDebugListenerReturnEvent )( struct WebDebugListener* pWebDebugListener, struct WebDebugState *pWebDebugState, intptr_t sourceId, int lineno, bool fromReturnCommand );
typedef WEBKIT_APOLLO_PROTO1 void ( WEBKIT_APOLLO_PROTO2 *WebDebugListenerWillExecuteProgram )( struct WebDebugListener* pWebDebugListener, struct WebDebugState *pWebDebugState, intptr_t sourceId, int lineno );
typedef WEBKIT_APOLLO_PROTO1 void ( WEBKIT_APOLLO_PROTO2 *WebDebugListenerDidExecuteProgram )( struct WebDebugListener* pWebDebugListener, struct WebDebugState *pWebDebugState, intptr_t sourceId, int lineno );
typedef WEBKIT_APOLLO_PROTO1 void ( WEBKIT_APOLLO_PROTO2 *WebDebugListenerSourceParsedFunctions )( struct WebDebugListener* pWebDebugListener, intptr_t sourceId, WebScriptProxyVariant * funcList );

struct WebDebugListenerVTable {
	unsigned long m_vTableSize;
	WebDebugListenerSourceParsed sourceParsed;
	WebDebugListenerException exception;
	WebDebugListenerAtStatement atStatement;
	WebDebugListenerCallEvent callEvent;
	WebDebugListenerReturnEvent returnEvent;
	WebDebugListenerWillExecuteProgram willExecuteProgram;
	WebDebugListenerDidExecuteProgram didExecuteProgram;
	WebDebugListenerSourceParsedFunctions sourceParsedFunctions;
};

struct WebDebugListener {
    const WebDebugListenerVTable* m_pVTable;
};

#endif
