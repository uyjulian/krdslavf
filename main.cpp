/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                    Copyright (C) 2019-2019 Julian Uy                    //
//                  https://sites.google.com/site/awertyb                  //
//                                                                         //
//   This program is free software; you can redistribute it and/or modify  //
//   it under the terms of the GNU General Public License as published by  //
//    the Free Software Foundation; either version 2 of the License, or    //
//                   (at your option) any later version.                   //
//                                                                         //
//     This program is distributed in the hope that it will be useful,     //
//     but WITHOUT ANY WARRANTY; without even the implied warranty of      //
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      //
//               GNU General Public License for more details.              //
//                                                                         //
// You should have received a copy of the GNU General Public License along //
// with this program; if not, write to the Free Software Foundation, Inc., //
//       51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#define INITGUID 1
#include "ncbind/ncbind.hpp"
#include <strmif.h>
#include <uuids.h>
#include <shlwapi.h>

DEFINE_GUID(CLSID_LAVSplitter, 0x171252A0, 0x8820, 0x4AFE, 0x9D, 0xF8, 0x5C, 0x92, 0xB2, 0xD6, 0x6B, 0x04);
DEFINE_GUID(CLSID_LAVVideo,    0xEE30215D, 0x164F, 0x4A92, 0xA4, 0xEB, 0x9D, 0x4C, 0x13, 0x39, 0x0F, 0x9F);
DEFINE_GUID(CLSID_LAVAudio,    0xE8E73B6B, 0x4CB3, 0x44A4, 0xBE, 0x99, 0x4F, 0x7B, 0xCB, 0x96, 0xE4, 0x91);

static const GUID mediauuid[] = {
	MEDIASUBTYPE_MPEG1System,
	MEDIASUBTYPE_MPEG1Video,
	MEDIASUBTYPE_MPEG1VideoCD,
	MEDIASUBTYPE_Avi,
	MEDIASUBTYPE_QTMovie,
};

static const tjs_char* mediatype[] = {
	TJS_W(".3ga"),
	TJS_W(".3gp"),
	TJS_W(".aac"),
	TJS_W(".ac3"),
	TJS_W(".amv"),
	TJS_W(".asf"),
	TJS_W(".avi"),
	TJS_W(".bdmv"),
	TJS_W(".bik"),
	TJS_W(".dat"),
	TJS_W(".divx"),
	TJS_W(".dts"),
	TJS_W(".dtshd"),
	TJS_W(".dvr-ms"),
	TJS_W(".eac3"),
	TJS_W(".evo"),
	TJS_W(".flac"),
	TJS_W(".flv"),
	TJS_W(".m1v"),
	TJS_W(".m2ts"),
	TJS_W(".m4v"),
	TJS_W(".mk3d"),
	TJS_W(".mka"),
	TJS_W(".mks"),
	TJS_W(".mkv"),
	TJS_W(".mov"),
	TJS_W(".mp3"),
	TJS_W(".mp4"),
	TJS_W(".mpeg"),
	TJS_W(".mpg"),
	TJS_W(".mpls"),
	TJS_W(".mpv"),
	TJS_W(".mts"),
	TJS_W(".mxf"),
	TJS_W(".ogg"),
	TJS_W(".ogm"),
	TJS_W(".ogv"),
	TJS_W(".qt"),
	TJS_W(".rm"),
	TJS_W(".rmvb"),
	TJS_W(".ssif"),
	TJS_W(".tp"),
	TJS_W(".ts"),
	TJS_W(".vob"),
	TJS_W(".vp6"),
	TJS_W(".webm"),
	TJS_W(".wmv"),
	TJS_W(".wtv"),
};

#ifdef TJS_64BIT_OS
#define PLUGIN_PATH "plugin64\\LAVFilters\\"
#else
#define PLUGIN_PATH "plugin\\LAVFilters\\"
#endif

#define DIRECTSHOW_FILTER_FACTORY_FUNCTION( name )  \
	HMODULE p##name##Module; \
	IClassFactory* p##name##Factory; \
	static void* tTVPCreate##name##Filter( void* formatdata ) { \
		IBaseFilter* pFilter; \
		if (!SUCCEEDED(p##name##Factory->CreateInstance(NULL, IID_IBaseFilter, (void**)&pFilter))) \
			TVPThrowExceptionMessage(TJS_W("krdslavfilters: could not create instance in " #name ".ax")); \
		return pFilter; \
	}

#define DIRECTSHOW_FILTER_INITIALIZATION( name ) \
	{ \
		TCHAR exePath [MAX_PATH]; \
		::GetModuleFileName(nullptr, exePath, sizeof( exePath ) / sizeof( exePath[0] )); \
		::PathRemoveFileSpec(exePath); \
		::PathAppend(exePath, TEXT(PLUGIN_PATH)); \
		TCHAR oldDllDir [MAX_PATH]; \
		::GetDllDirectory( sizeof( oldDllDir ) / sizeof( oldDllDir[0] ), oldDllDir ); \
		::SetDllDirectory( exePath ); \
		p##name##Module = ::LoadLibrary(TEXT(#name ".ax")); \
		::SetDllDirectory( oldDllDir ); \
		if (!p##name##Module) \
			TVPThrowExceptionMessage(TJS_W("krdslavfilters: failed to load " #name ".ax")); \
		HRESULT (WINAPI *cbFactory)(REFCLSID, REFIID, void**) = (HRESULT (WINAPI *)(REFCLSID, REFIID, void**))::GetProcAddress(p##name##Module, "DllGetClassObject"); \
		if (!cbFactory) \
			TVPThrowExceptionMessage(TJS_W("krdslavfilters: could not find DllGetClassObject symbol in " #name ".ax")); \
		if (!SUCCEEDED(cbFactory(CLSID_##name, IID_IClassFactory, (void**)&p##name##Factory))) \
			TVPThrowExceptionMessage(TJS_W("krdslavfilters: could not retrieve factory in " #name ".ax")); \
	}

#define DIRECTSHOW_FILTER_CLEANUP( name ) \
	{ \
		if (p##name##Factory) \
			p##name##Factory->Release(); \
		p##name##Factory = nullptr; \
		if (p##name##Module) \
			::FreeLibrary(p##name##Module); \
		p##name##Module = nullptr; \
	}

DIRECTSHOW_FILTER_FACTORY_FUNCTION(LAVSplitter);
DIRECTSHOW_FILTER_FACTORY_FUNCTION(LAVVideo);
DIRECTSHOW_FILTER_FACTORY_FUNCTION(LAVAudio);

static void regcb() {
	DIRECTSHOW_FILTER_INITIALIZATION(LAVSplitter);
	DIRECTSHOW_FILTER_INITIALIZATION(LAVVideo);
	DIRECTSHOW_FILTER_INITIALIZATION(LAVAudio);
	for (size_t i = 0; i < sizeof(mediauuid)/sizeof(mediauuid[0]); i += 1)
		for (size_t j = 0; j < sizeof(mediatype)/sizeof(mediatype[0]); j += 1)
			TVPRegisterDSVideoCodec( mediatype[j], (void*)&mediauuid[i], tTVPCreateLAVSplitterFilter, tTVPCreateLAVVideoFilter, tTVPCreateLAVAudioFilter, NULL );
}

NCB_PRE_REGIST_CALLBACK(regcb);

static void unregcb() {
	for (size_t i = 0; i < sizeof(mediauuid)/sizeof(mediauuid[0]); i += 1)
		for (size_t j = 0; j < sizeof(mediatype)/sizeof(mediatype[0]); j += 1)
			TVPUnregisterDSVideoCodec( mediatype[j], (void*)&mediauuid[i], tTVPCreateLAVSplitterFilter, tTVPCreateLAVVideoFilter, tTVPCreateLAVAudioFilter, NULL );
	DIRECTSHOW_FILTER_CLEANUP(LAVSplitter);
	DIRECTSHOW_FILTER_CLEANUP(LAVVideo);
	DIRECTSHOW_FILTER_CLEANUP(LAVAudio);
}

NCB_POST_UNREGIST_CALLBACK(unregcb);
