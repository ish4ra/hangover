/*
 * Copyright 2017 Stefan Dösinger for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* NOTE: The guest side uses mingw's headers. The host side uses Wine's headers. */

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>

#include "windows-user-services.h"
#include "dll_list.h"
#include "qemu_d3d9.h"

#ifdef QEMU_DLL_GUEST

BOOL WINAPI DllMain(HMODULE mod, DWORD reason, void *reserved)
{
    return TRUE;
}

#else

#include <wine/debug.h>
WINE_DEFAULT_DEBUG_CHANNEL(qemu_d3d9);

const struct qemu_ops *qemu_ops;

static const syscall_handler dll_functions[] =
{
    qemu_d3d9_AddRef,
    qemu_d3d9_CheckDepthStencilMatch,
    qemu_d3d9_CheckDeviceFormat,
    qemu_d3d9_CheckDeviceFormatConversion,
    qemu_d3d9_CheckDeviceMultiSampleType,
    qemu_d3d9_CheckDeviceType,
    qemu_d3d9_CreateDevice,
    qemu_d3d9_CreateDeviceEx,
    qemu_d3d9_device_AddRef,
    qemu_d3d9_device_BeginScene,
    qemu_d3d9_device_BeginStateBlock,
    qemu_d3d9_device_CheckDeviceState,
    qemu_d3d9_device_CheckResourceResidency,
    qemu_d3d9_device_Clear,
    qemu_d3d9_device_ColorFill,
    qemu_d3d9_device_ComposeRects,
    qemu_d3d9_device_CreateAdditionalSwapChain,
    qemu_d3d9_device_CreateCubeTexture,
    qemu_d3d9_device_CreateDepthStencilSurface,
    qemu_d3d9_device_CreateDepthStencilSurfaceEx,
    qemu_d3d9_device_CreateIndexBuffer,
    qemu_d3d9_device_CreateOffscreenPlainSurface,
    qemu_d3d9_device_CreateOffscreenPlainSurfaceEx,
    qemu_d3d9_device_CreatePixelShader,
    qemu_d3d9_device_CreateQuery,
    qemu_d3d9_device_CreateRenderTarget,
    qemu_d3d9_device_CreateRenderTargetEx,
    qemu_d3d9_device_CreateStateBlock,
    qemu_d3d9_device_CreateTexture,
    qemu_d3d9_device_CreateVertexBuffer,
    qemu_d3d9_device_CreateVertexDeclaration,
    qemu_d3d9_device_CreateVertexShader,
    qemu_d3d9_device_CreateVolumeTexture,
    qemu_d3d9_device_DeletePatch,
    qemu_d3d9_device_DrawIndexedPrimitive,
    qemu_d3d9_device_DrawIndexedPrimitiveUP,
    qemu_d3d9_device_DrawPrimitive,
    qemu_d3d9_device_DrawPrimitiveUP,
    qemu_d3d9_device_DrawRectPatch,
    qemu_d3d9_device_DrawTriPatch,
    qemu_d3d9_device_EndScene,
    qemu_d3d9_device_EndStateBlock,
    qemu_d3d9_device_EvictManagedResources,
    qemu_d3d9_device_GetAvailableTextureMem,
    qemu_d3d9_device_GetBackBuffer,
    qemu_d3d9_device_GetClipPlane,
    qemu_d3d9_device_GetClipStatus,
    qemu_d3d9_device_GetCreationParameters,
    qemu_d3d9_device_GetCurrentTexturePalette,
    qemu_d3d9_device_GetDepthStencilSurface,
    qemu_d3d9_device_GetDeviceCaps,
    qemu_d3d9_device_GetDirect3D,
    qemu_d3d9_device_GetDisplayMode,
    qemu_d3d9_device_GetDisplayModeEx,
    qemu_d3d9_device_GetFrontBufferData,
    qemu_d3d9_device_GetFVF,
    qemu_d3d9_device_GetGammaRamp,
    qemu_d3d9_device_GetGPUThreadPriority,
    qemu_d3d9_device_GetIndices,
    qemu_d3d9_device_GetLight,
    qemu_d3d9_device_GetLightEnable,
    qemu_d3d9_device_GetMaterial,
    qemu_d3d9_device_GetMaximumFrameLatency,
    qemu_d3d9_device_GetNPatchMode,
    qemu_d3d9_device_GetNumberOfSwapChains,
    qemu_d3d9_device_GetPaletteEntries,
    qemu_d3d9_device_GetPixelShader,
    qemu_d3d9_device_GetPixelShaderConstantB,
    qemu_d3d9_device_GetPixelShaderConstantF,
    qemu_d3d9_device_GetPixelShaderConstantI,
    qemu_d3d9_device_GetRasterStatus,
    qemu_d3d9_device_GetRenderState,
    qemu_d3d9_device_GetRenderTarget,
    qemu_d3d9_device_GetRenderTargetData,
    qemu_d3d9_device_GetSamplerState,
    qemu_d3d9_device_GetScissorRect,
    qemu_d3d9_device_GetSoftwareVertexProcessing,
    qemu_d3d9_device_GetStreamSource,
    qemu_d3d9_device_GetStreamSourceFreq,
    qemu_d3d9_device_GetSwapChain,
    qemu_d3d9_device_GetTexture,
    qemu_d3d9_device_GetTextureStageState,
    qemu_d3d9_device_GetTransform,
    qemu_d3d9_device_GetVertexDeclaration,
    qemu_d3d9_device_GetVertexShader,
    qemu_d3d9_device_GetVertexShaderConstantB,
    qemu_d3d9_device_GetVertexShaderConstantF,
    qemu_d3d9_device_GetVertexShaderConstantI,
    qemu_d3d9_device_GetViewport,
    qemu_d3d9_device_LightEnable,
    qemu_d3d9_device_MultiplyTransform,
    qemu_d3d9_device_Present,
    qemu_d3d9_device_PresentEx,
    qemu_d3d9_device_ProcessVertices,
    qemu_d3d9_device_QueryInterface,
    qemu_d3d9_device_Release,
    qemu_d3d9_device_Reset,
    qemu_d3d9_device_ResetEx,
    qemu_d3d9_device_SetClipPlane,
    qemu_d3d9_device_SetClipStatus,
    qemu_d3d9_device_SetConvolutionMonoKernel,
    qemu_d3d9_device_SetCurrentTexturePalette,
    qemu_d3d9_device_SetCursorPosition,
    qemu_d3d9_device_SetCursorProperties,
    qemu_d3d9_device_SetDepthStencilSurface,
    qemu_d3d9_device_SetDialogBoxMode,
    qemu_d3d9_device_SetFVF,
    qemu_d3d9_device_SetGammaRamp,
    qemu_d3d9_device_SetGPUThreadPriority,
    qemu_d3d9_device_SetIndices,
    qemu_d3d9_device_SetLight,
    qemu_d3d9_device_SetMaterial,
    qemu_d3d9_device_SetMaximumFrameLatency,
    qemu_d3d9_device_SetNPatchMode,
    qemu_d3d9_device_SetPaletteEntries,
    qemu_d3d9_device_SetPixelShader,
    qemu_d3d9_device_SetPixelShaderConstantB,
    qemu_d3d9_device_SetPixelShaderConstantF,
    qemu_d3d9_device_SetPixelShaderConstantI,
    qemu_d3d9_device_SetRenderState,
    qemu_d3d9_device_SetRenderTarget,
    qemu_d3d9_device_SetSamplerState,
    qemu_d3d9_device_SetScissorRect,
    qemu_d3d9_device_SetSoftwareVertexProcessing,
    qemu_d3d9_device_SetStreamSource,
    qemu_d3d9_device_SetStreamSourceFreq,
    qemu_d3d9_device_SetTexture,
    qemu_d3d9_device_SetTextureStageState,
    qemu_d3d9_device_SetTransform,
    qemu_d3d9_device_SetVertexDeclaration,
    qemu_d3d9_device_SetVertexShader,
    qemu_d3d9_device_SetVertexShaderConstantB,
    qemu_d3d9_device_SetVertexShaderConstantF,
    qemu_d3d9_device_SetVertexShaderConstantI,
    qemu_d3d9_device_SetViewport,
    qemu_d3d9_device_ShowCursor,
    qemu_d3d9_device_StretchRect,
    qemu_d3d9_device_TestCooperativeLevel,
    qemu_d3d9_device_UpdateSurface,
    qemu_d3d9_device_UpdateTexture,
    qemu_d3d9_device_ValidateDevice,
    qemu_d3d9_device_WaitForVBlank,
    qemu_d3d9_EnumAdapterModes,
    qemu_d3d9_EnumAdapterModesEx,
    qemu_d3d9_GetAdapterCount,
    qemu_d3d9_GetAdapterDisplayMode,
    qemu_d3d9_GetAdapterDisplayModeEx,
    qemu_d3d9_GetAdapterIdentifier,
    qemu_d3d9_GetAdapterLUID,
    qemu_d3d9_GetAdapterModeCount,
    qemu_d3d9_GetAdapterModeCountEx,
    qemu_d3d9_GetAdapterMonitor,
    qemu_d3d9_GetDeviceCaps,
    qemu_d3d9_QueryInterface,
    qemu_d3d9_RegisterSoftwareDevice,
    qemu_d3d9_Release,
    qemu_d3d9_swapchain_AddRef,
    qemu_d3d9_swapchain_GetBackBuffer,
    qemu_d3d9_swapchain_GetDevice,
    qemu_d3d9_swapchain_GetDisplayMode,
    qemu_d3d9_swapchain_GetDisplayModeEx,
    qemu_d3d9_swapchain_GetFrontBufferData,
    qemu_d3d9_swapchain_GetLastPresentCount,
    qemu_d3d9_swapchain_GetPresentParameters,
    qemu_d3d9_swapchain_GetPresentStatistics,
    qemu_d3d9_swapchain_GetRasterStatus,
    qemu_d3d9_swapchain_Present,
    qemu_d3d9_swapchain_QueryInterface,
    qemu_d3d9_swapchain_Release,
    qemu_D3DPERF_BeginEvent,
    qemu_D3DPERF_EndEvent,
    qemu_D3DPERF_GetStatus,
    qemu_D3DPERF_QueryRepeatFrame,
    qemu_D3DPERF_SetMarker,
    qemu_D3DPERF_SetOptions,
    qemu_D3DPERF_SetRegion,
    qemu_DebugSetMute,
    qemu_Direct3DCreate9,
    qemu_Direct3DCreate9Ex,
    qemu_Direct3DShaderValidatorCreate9,
};

const WINAPI syscall_handler *qemu_dll_register(const struct qemu_ops *ops, uint32_t *dll_num)
{
    WINE_TRACE("Loading host-side d3d9 wrapper.\n");

    qemu_ops = ops;
    *dll_num = QEMU_CURRENT_DLL;

    return dll_functions;
}

#endif
