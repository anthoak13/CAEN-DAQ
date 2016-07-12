// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME builddIdictdIdictionary

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/home/adam/Documents/reu/CAEN-DAQ/include/DataProcessor.h"
#include "/home/adam/Documents/reu/CAEN-DAQ/include/WavedumpConfig.h"
#include "/home/adam/Documents/reu/CAEN-DAQ/include/Digitizer.h"
#include "/home/adam/Documents/reu/CAEN-DAQ/include/SignalProcessor.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_DataProcessor(void *p = 0);
   static void *newArray_DataProcessor(Long_t size, void *p);
   static void delete_DataProcessor(void *p);
   static void deleteArray_DataProcessor(void *p);
   static void destruct_DataProcessor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DataProcessor*)
   {
      ::DataProcessor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DataProcessor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DataProcessor", ::DataProcessor::Class_Version(), "include/DataProcessor.h", 19,
                  typeid(::DataProcessor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DataProcessor::Dictionary, isa_proxy, 4,
                  sizeof(::DataProcessor) );
      instance.SetNew(&new_DataProcessor);
      instance.SetNewArray(&newArray_DataProcessor);
      instance.SetDelete(&delete_DataProcessor);
      instance.SetDeleteArray(&deleteArray_DataProcessor);
      instance.SetDestructor(&destruct_DataProcessor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DataProcessor*)
   {
      return GenerateInitInstanceLocal((::DataProcessor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DataProcessor*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_SignalProcessor(void *p = 0);
   static void *newArray_SignalProcessor(Long_t size, void *p);
   static void delete_SignalProcessor(void *p);
   static void deleteArray_SignalProcessor(void *p);
   static void destruct_SignalProcessor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SignalProcessor*)
   {
      ::SignalProcessor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SignalProcessor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SignalProcessor", ::SignalProcessor::Class_Version(), "include/SignalProcessor.h", 19,
                  typeid(::SignalProcessor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SignalProcessor::Dictionary, isa_proxy, 4,
                  sizeof(::SignalProcessor) );
      instance.SetNew(&new_SignalProcessor);
      instance.SetNewArray(&newArray_SignalProcessor);
      instance.SetDelete(&delete_SignalProcessor);
      instance.SetDeleteArray(&deleteArray_SignalProcessor);
      instance.SetDestructor(&destruct_SignalProcessor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SignalProcessor*)
   {
      return GenerateInitInstanceLocal((::SignalProcessor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SignalProcessor*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ChannelConfig(void *p = 0);
   static void *newArray_ChannelConfig(Long_t size, void *p);
   static void delete_ChannelConfig(void *p);
   static void deleteArray_ChannelConfig(void *p);
   static void destruct_ChannelConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ChannelConfig*)
   {
      ::ChannelConfig *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ChannelConfig >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ChannelConfig", ::ChannelConfig::Class_Version(), "include/WavedumpConfig.h", 32,
                  typeid(::ChannelConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ChannelConfig::Dictionary, isa_proxy, 4,
                  sizeof(::ChannelConfig) );
      instance.SetNew(&new_ChannelConfig);
      instance.SetNewArray(&newArray_ChannelConfig);
      instance.SetDelete(&delete_ChannelConfig);
      instance.SetDeleteArray(&deleteArray_ChannelConfig);
      instance.SetDestructor(&destruct_ChannelConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ChannelConfig*)
   {
      return GenerateInitInstanceLocal((::ChannelConfig*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ChannelConfig*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_WavedumpConfig(void *p = 0);
   static void *newArray_WavedumpConfig(Long_t size, void *p);
   static void delete_WavedumpConfig(void *p);
   static void deleteArray_WavedumpConfig(void *p);
   static void destruct_WavedumpConfig(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::WavedumpConfig*)
   {
      ::WavedumpConfig *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::WavedumpConfig >(0);
      static ::ROOT::TGenericClassInfo 
         instance("WavedumpConfig", ::WavedumpConfig::Class_Version(), "include/WavedumpConfig.h", 72,
                  typeid(::WavedumpConfig), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::WavedumpConfig::Dictionary, isa_proxy, 4,
                  sizeof(::WavedumpConfig) );
      instance.SetNew(&new_WavedumpConfig);
      instance.SetNewArray(&newArray_WavedumpConfig);
      instance.SetDelete(&delete_WavedumpConfig);
      instance.SetDeleteArray(&deleteArray_WavedumpConfig);
      instance.SetDestructor(&destruct_WavedumpConfig);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::WavedumpConfig*)
   {
      return GenerateInitInstanceLocal((::WavedumpConfig*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::WavedumpConfig*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DataProcessor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DataProcessor::Class_Name()
{
   return "DataProcessor";
}

//______________________________________________________________________________
const char *DataProcessor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DataProcessor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DataProcessor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DataProcessor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DataProcessor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DataProcessor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DataProcessor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DataProcessor*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SignalProcessor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SignalProcessor::Class_Name()
{
   return "SignalProcessor";
}

//______________________________________________________________________________
const char *SignalProcessor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SignalProcessor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SignalProcessor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SignalProcessor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SignalProcessor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SignalProcessor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SignalProcessor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SignalProcessor*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ChannelConfig::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ChannelConfig::Class_Name()
{
   return "ChannelConfig";
}

//______________________________________________________________________________
const char *ChannelConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ChannelConfig*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ChannelConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ChannelConfig*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ChannelConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ChannelConfig*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ChannelConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ChannelConfig*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr WavedumpConfig::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *WavedumpConfig::Class_Name()
{
   return "WavedumpConfig";
}

//______________________________________________________________________________
const char *WavedumpConfig::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WavedumpConfig*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int WavedumpConfig::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WavedumpConfig*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WavedumpConfig::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WavedumpConfig*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WavedumpConfig::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WavedumpConfig*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DataProcessor::Streamer(TBuffer &R__b)
{
   // Stream an object of class DataProcessor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DataProcessor::Class(),this);
   } else {
      R__b.WriteClassBuffer(DataProcessor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DataProcessor(void *p) {
      return  p ? new(p) ::DataProcessor : new ::DataProcessor;
   }
   static void *newArray_DataProcessor(Long_t nElements, void *p) {
      return p ? new(p) ::DataProcessor[nElements] : new ::DataProcessor[nElements];
   }
   // Wrapper around operator delete
   static void delete_DataProcessor(void *p) {
      delete ((::DataProcessor*)p);
   }
   static void deleteArray_DataProcessor(void *p) {
      delete [] ((::DataProcessor*)p);
   }
   static void destruct_DataProcessor(void *p) {
      typedef ::DataProcessor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DataProcessor

//______________________________________________________________________________
void SignalProcessor::Streamer(TBuffer &R__b)
{
   // Stream an object of class SignalProcessor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(SignalProcessor::Class(),this);
   } else {
      R__b.WriteClassBuffer(SignalProcessor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SignalProcessor(void *p) {
      return  p ? new(p) ::SignalProcessor : new ::SignalProcessor;
   }
   static void *newArray_SignalProcessor(Long_t nElements, void *p) {
      return p ? new(p) ::SignalProcessor[nElements] : new ::SignalProcessor[nElements];
   }
   // Wrapper around operator delete
   static void delete_SignalProcessor(void *p) {
      delete ((::SignalProcessor*)p);
   }
   static void deleteArray_SignalProcessor(void *p) {
      delete [] ((::SignalProcessor*)p);
   }
   static void destruct_SignalProcessor(void *p) {
      typedef ::SignalProcessor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SignalProcessor

//______________________________________________________________________________
void ChannelConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class ChannelConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ChannelConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(ChannelConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ChannelConfig(void *p) {
      return  p ? new(p) ::ChannelConfig : new ::ChannelConfig;
   }
   static void *newArray_ChannelConfig(Long_t nElements, void *p) {
      return p ? new(p) ::ChannelConfig[nElements] : new ::ChannelConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_ChannelConfig(void *p) {
      delete ((::ChannelConfig*)p);
   }
   static void deleteArray_ChannelConfig(void *p) {
      delete [] ((::ChannelConfig*)p);
   }
   static void destruct_ChannelConfig(void *p) {
      typedef ::ChannelConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ChannelConfig

//______________________________________________________________________________
void WavedumpConfig::Streamer(TBuffer &R__b)
{
   // Stream an object of class WavedumpConfig.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(WavedumpConfig::Class(),this);
   } else {
      R__b.WriteClassBuffer(WavedumpConfig::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_WavedumpConfig(void *p) {
      return  p ? new(p) ::WavedumpConfig : new ::WavedumpConfig;
   }
   static void *newArray_WavedumpConfig(Long_t nElements, void *p) {
      return p ? new(p) ::WavedumpConfig[nElements] : new ::WavedumpConfig[nElements];
   }
   // Wrapper around operator delete
   static void delete_WavedumpConfig(void *p) {
      delete ((::WavedumpConfig*)p);
   }
   static void deleteArray_WavedumpConfig(void *p) {
      delete [] ((::WavedumpConfig*)p);
   }
   static void destruct_WavedumpConfig(void *p) {
      typedef ::WavedumpConfig current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::WavedumpConfig

namespace {
  void TriggerDictionaryInitialization_dictionary_Impl() {
    static const char* headers[] = {
"include/DataProcessor.h",
"include/WavedumpConfig.h",
"include/Digitizer.h",
"include/SignalProcessor.h",
0
    };
    static const char* includePaths[] = {
"/root-6.06.02/include",
"/home/adam/Documents/reu/CAEN-DAQ/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "dictionary dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$include/DataProcessor.h")))  DataProcessor;
class __attribute__((annotate("$clingAutoload$include/DataProcessor.h")))  SignalProcessor;
class __attribute__((annotate("$clingAutoload$include/WavedumpConfig.h")))  ChannelConfig;
class __attribute__((annotate("$clingAutoload$include/WavedumpConfig.h")))  WavedumpConfig;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "dictionary dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "include/DataProcessor.h"
#include "include/WavedumpConfig.h"
#include "include/Digitizer.h"
#include "include/SignalProcessor.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ChannelConfig", payloadCode, "@",
"ChannelTrigger", payloadCode, "@",
"DataProcessor", payloadCode, "@",
"SignalProcessor", payloadCode, "@",
"WavedumpConfig", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictionary",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictionary_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictionary_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictionary() {
  TriggerDictionaryInitialization_dictionary_Impl();
}
