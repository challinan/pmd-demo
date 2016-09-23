

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from pmd.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef pmd_464398885_h
#define pmd_464398885_h

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#else
#include "ndds_standalone_type.h"
#endif

namespace DataBus {

    extern const char *pm_data_structTYPENAME;

    struct pm_data_structSeq;
    #ifndef NDDS_STANDALONE_TYPE
    class pm_data_structTypeSupport;
    class pm_data_structDataWriter;
    class pm_data_structDataReader;
    #endif

    class pm_data_struct 
    {
      public:
        typedef struct pm_data_structSeq Seq;
        #ifndef NDDS_STANDALONE_TYPE
        typedef pm_data_structTypeSupport TypeSupport;
        typedef pm_data_structDataWriter DataWriter;
        typedef pm_data_structDataReader DataReader;
        #endif

        DDS_Long   ecgValue ;
        DDS_Long   abpValue ;
        DDS_Long   plethValue ;

    };
    #if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
    /* If the code is building on Windows, start exporting symbols.
    */
    #undef NDDSUSERDllExport
    #define NDDSUSERDllExport __declspec(dllexport)
    #endif

    NDDSUSERDllExport DDS_TypeCode* pm_data_struct_get_typecode(void); /* Type code */

    DDS_SEQUENCE(pm_data_structSeq, pm_data_struct);                                        

    NDDSUSERDllExport
    RTIBool pm_data_struct_initialize(
        pm_data_struct* self);

    NDDSUSERDllExport
    RTIBool pm_data_struct_initialize_ex(
        pm_data_struct* self,RTIBool allocatePointers,RTIBool allocateMemory);

    NDDSUSERDllExport
    RTIBool pm_data_struct_initialize_w_params(
        pm_data_struct* self,
        const struct DDS_TypeAllocationParams_t * allocParams);        

    NDDSUSERDllExport
    void pm_data_struct_finalize(
        pm_data_struct* self);

    NDDSUSERDllExport
    void pm_data_struct_finalize_ex(
        pm_data_struct* self,RTIBool deletePointers);

    NDDSUSERDllExport
    void pm_data_struct_finalize_w_params(
        pm_data_struct* self,
        const struct DDS_TypeDeallocationParams_t * deallocParams);

    NDDSUSERDllExport
    void pm_data_struct_finalize_optional_members(
        pm_data_struct* self, RTIBool deletePointers);  

    NDDSUSERDllExport
    RTIBool pm_data_struct_copy(
        pm_data_struct* dst,
        const pm_data_struct* src);

    #if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
    /* If the code is building on Windows, stop exporting symbols.
    */
    #undef NDDSUSERDllExport
    #define NDDSUSERDllExport
    #endif
} /* namespace DataBus  */

#endif /* pmd */

