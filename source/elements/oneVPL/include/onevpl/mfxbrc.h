/*############################################################################
  # Copyright (C) 2019-2020 Intel Corporation
  #
  # SPDX-License-Identifier: MIT
  ############################################################################*/

#ifndef __MFXBRC_H__
#define __MFXBRC_H__

#include "mfxstructures.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*! See the mfxExtBRC structure for details. */
enum {
    MFX_EXTBUFF_BRC = MFX_MAKEFOURCC('E','B','R','C')
};

MFX_PACK_BEGIN_STRUCT_W_PTR()
/*!
   The mfxBRCFrameParam structure describes frame parameters required for external BRC functions.
*/
typedef struct {
#if (MFX_VERSION >= 1026)
    mfxU32 reserved[23];
    mfxU16 SceneChange;     /*!< Frame belongs to a new scene if non zero. */
    mfxU16 LongTerm;        /*!< Frame is a Long Term Reference frame if non zero. */
    mfxU32 FrameCmplx;      /*!< Frame Complexity Frame spatial complexity if non zero. Zero if complexity is not available. */
#else
    mfxU32 reserved[25];
#endif
    mfxU32 EncodedOrder;    /*!< The frame number in a sequence of reordered frames starting from encoder Init. */
    mfxU32 DisplayOrder;    /*!< The frame number in a sequence of frames in display order starting from last IDR. */
    mfxU32 CodedFrameSize;  /*!< Size of the frame in bytes after encoding. */
    mfxU16 FrameType;       /*!< See FrameType enumerator */
    mfxU16 PyramidLayer;    /*!< B-pyramid or P-pyramid layer that the frame belongs to. */
    mfxU16 NumRecode;       /*!< Number of recodings performed for this frame. */
    mfxU16 NumExtParam;     /*!< Reserved for future use. */
    mfxExtBuffer** ExtParam;/*!< Reserved for future use. */
} mfxBRCFrameParam;
MFX_PACK_END()

MFX_PACK_BEGIN_STRUCT_W_PTR()
/*!
   The mfxBRCFrameCtrl structure specifies controls for next frame encoding provided by external BRC functions.
*/
typedef struct {
    mfxI32 QpY;                     /*!< Frame-level Luma QP. */
#if (MFX_VERSION >= 1029)
    mfxU32 InitialCpbRemovalDelay;  /*!< See initial_cpb_removal_delay in codec standard. Ignored if no HRD control:
                                         mfxExtCodingOption::VuiNalHrdParameters = MFX_CODINGOPTION_OFF. Calculated by encoder if
                                         initial_cpb_removal_delay==0 && initial_cpb_removal_offset == 0 && HRD control is switched on. */
    mfxU32 InitialCpbRemovalOffset; /*!< See initial_cpb_removal_offset in codec standard. Ignored if no HRD control:
                                         mfxExtCodingOption::VuiNalHrdParameters = MFX_CODINGOPTION_OFF. Calculated by encoder if
                                         initial_cpb_removal_delay==0 && initial_cpb_removal_offset == 0 && HRD control is switched on. */
    mfxU32 reserved1[7];
    mfxU32 MaxFrameSize;            /*!< Max frame size in bytes. Option for repack feature. Driver calls PAK until current frame size is
                                         less than or equal to maxFrameSize, or number of repacking for this frame is equal to maxNumRePak. Repack is available
                                         if there is driver support, MaxFrameSize !=0, MaxNumRePak != 0. Ignored if maxNumRePak == 0. */
    mfxU8  DeltaQP[8];              /*!< Option for repack feature. Ignored if maxNumRePak == 0 or maxNumRePak==0. If current
                                         frame size > maxFrameSize and or number of repacking (nRepack) for this frame <= maxNumRePak,
                                         PAK is called with QP = mfxBRCFrameCtrl::QpY + Sum(DeltaQP[i]), where i = [0,nRepack].
                                         Non zero DeltaQP[nRepack] are ignored if nRepack > maxNumRePak.
                                         If repacking feature is on ( maxFrameSize & maxNumRePak are not zero), it is calculated by encoder. */
    mfxU16 MaxNumRepak;             /*!< Number of possible repacks in driver if current frame size > maxFrameSize. Ignored if maxFrameSize==0.
                                         See maxFrameSize description. Possible values are [0,8]. */
    mfxU16 NumExtParam;             /*!< Reserved for future use. */
    mfxExtBuffer** ExtParam;        /*!< Reserved for future use. */
#else
    mfxU32 reserved1[13];
    mfxHDL reserved2;
#endif
} mfxBRCFrameCtrl;
MFX_PACK_END()

/*! The BRCStatus enumerator itemizes instructions to the SDK encoder by mfxExtBrc::Update. */
enum {
    MFX_BRC_OK                = 0, /*!< CodedFrameSize is acceptable, no further recoding/padding/skip required, proceed to next frame. */
    MFX_BRC_BIG_FRAME         = 1, /*!< Coded frame is too big, recoding required. */
    MFX_BRC_SMALL_FRAME       = 2, /*!< Coded frame is too small, recoding required. */
    MFX_BRC_PANIC_BIG_FRAME   = 3, /*!< Coded frame is too big, no further recoding possible - skip frame. */
    MFX_BRC_PANIC_SMALL_FRAME = 4  /*!< Coded frame is too small, no further recoding possible - required padding to mfxBRCFrameStatus::MinFrameSize. */
};

MFX_PACK_BEGIN_USUAL_STRUCT()
/*!
   The mfxBRCFrameStatus structure specifies instructions for the SDK encoder provided by external BRC after each frame encoding. See the BRCStatus enumerator for details.
*/
typedef struct {
    mfxU32 MinFrameSize;    /*!< Size in bytes, coded frame must be padded to when Status = MFX_BRC_PANIC_SMALL_FRAME. */
    mfxU16 BRCStatus;       /*!< See BRCStatus enumerator. */
    mfxU16 reserved[25];
    mfxHDL reserved1;
} mfxBRCFrameStatus;
MFX_PACK_END()

MFX_PACK_BEGIN_STRUCT_W_PTR()
/*!
   The mfxExtBRC structure contains a set of callbacks to perform external bitrate control. Can be attached to mfxVideoParam structure during
   encoder initialization. Turn mfxExtCodingOption2::ExtBRC option ON to make the encoder use the external BRC instead of the native one.
*/
typedef struct {
    mfxExtBuffer Header; /*!< Extension buffer header. Header.BufferId must be equal to MFX_EXTBUFF_BRC. */

    mfxU32 reserved[14];
    mfxHDL pthis;        /*!< Pointer to the BRC object. */

    /*!
       @brief This function initializes the BRC session according to parameters from input mfxVideoParam and attached structures. It does not modify the input mfxVideoParam and attached structures. Invoked during MFXVideoENCODE_Init.

       @param[in]      pthis Pointer to the BRC object.
       @param[in]      par   Pointer to the mfxVideoParam structure that was used for the encoder initialization.

       @return
          MFX_ERR_NONE               The function completed successfully. \n
          MFX_ERR_UNSUPPORTED        The function detected unsupported video parameters.
    */
    mfxStatus (MFX_CDECL *Init)         (mfxHDL pthis, mfxVideoParam* par);

    /*!
       @brief This function resets BRC session according to new parameters. It does not modify the input mfxVideoParam and attached structures. Invoked during MFXVideoENCODE_Reset.

       @param[in]      pthis Pointer to the BRC object.
       @param[in]      par   Pointer to the mfxVideoParam structure that was used for the encoder initialization.

       @return
          MFX_ERR_NONE                       The function completed successfully. \n
          MFX_ERR_UNSUPPORTED                The function detected unsupported video parameters. \n
          MFX_ERR_INCOMPATIBLE_VIDEO_PARAM   The function detected that the video parameters provided by the application are incompatible with
                                             initialization parameters. Reset requires additional memory allocation and cannot be executed.
    */
    mfxStatus (MFX_CDECL *Reset)        (mfxHDL pthis, mfxVideoParam* par);

    /*!
       @brief This function de-allocates any internal resources acquired in Init for this BRC session. Invoked during MFXVideoENCODE_Close.

       @param[in]      pthis Pointer to the BRC object.

       @return
          MFX_ERR_NONE               The function completed successfully.
    */
    mfxStatus (MFX_CDECL *Close)        (mfxHDL pthis);

    /*! @brief This function returns controls (ctrl) to encode next frame based on info from input mfxBRCFrameParam structure (par) and
               internal BRC state. Invoked asynchronously before each frame encoding or recoding.

       @param[in]      pthis Pointer to the BRC object.
       @param[in]      par   Pointer to the mfxVideoParam structure that was used for the encoder initialization.
       @param[out]     ctrl  Pointer to the output mfxBRCFrameCtrl structure.

       @return
          MFX_ERR_NONE               The function completed successfully.
    */
    mfxStatus (MFX_CDECL* GetFrameCtrl) (mfxHDL pthis, mfxBRCFrameParam* par, mfxBRCFrameCtrl* ctrl);

    /*!
       @brief This function updates internal BRC state and returns status to instruct encoder whether it should recode the previous frame,
               skip it, do padding, or proceed to next frame based on info from input mfxBRCFrameParam and mfxBRCFrameCtrl structures.
               Invoked asynchronously after each frame encoding or recoding.

       @param[in]      pthis Pointer to the BRC object.
       @param[in]      par   Pointer to the mfxVideoParam structure that was used for the encoder initialization.
       @param[in]      ctrl  Pointer to the output mfxBRCFrameCtrl structure.
       @param[in]    status  Pointer to the output mfxBRCFrameStatus structure.


       @return
          MFX_ERR_NONE               The function completed successfully.
    */
    mfxStatus (MFX_CDECL* Update)       (mfxHDL pthis, mfxBRCFrameParam* par, mfxBRCFrameCtrl* ctrl, mfxBRCFrameStatus* status);

    mfxHDL reserved1[10];
} mfxExtBRC;
MFX_PACK_END()

#ifdef __cplusplus
} // extern "C"
#endif /* __cplusplus */

#endif

