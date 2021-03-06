#include "webgpu.h"

/*
 * WebGPU specific header added in 1.39.18.
 *
 * TODO: remove eventually
 */
#if __EMSCRIPTEN_major__ == 1 && (__EMSCRIPTEN_minor__ < 39 || (__EMSCRIPTEN_minor__ == 39 && __EMSCRIPTEN_tiny__ < 18))
#include <emscripten/html5.h>
#else
#include <emscripten/html5_webgpu.h>
#endif

//******************************** Public API ********************************/

WGPUDevice webgpu::create(window::Handle /*window*/, WGPUBackendType /*type*/) {
	/*
	 * Before calling this needs to have the WebGPU device pre-inited from JS,
	 * which is an async call so needs some magic to delay running the main()
	 * until after the promise is resolved.
	 */
	return emscripten_webgpu_get_device();
}

WGPUSwapChain webgpu::createSwapChain(WGPUDevice device) {
	WGPUSurfaceDescriptorFromHTMLCanvasId canvDesc = {};
	canvDesc.chain.sType = WGPUSType_SurfaceDescriptorFromHTMLCanvasId;
	canvDesc.id = "canvas";
	
	WGPUSurfaceDescriptor surfDesc = {};
	surfDesc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&canvDesc);
	
	WGPUSurface surface = wgpuInstanceCreateSurface(nullptr, &surfDesc);
	
	WGPUSwapChainDescriptor swapDesc = {};
	swapDesc.usage  = WGPUTextureUsage_OutputAttachment;
	swapDesc.format = WGPUTextureFormat_BGRA8Unorm;
	swapDesc.width  = 800;
	swapDesc.height = 450;
	swapDesc.presentMode = WGPUPresentMode_Fifo;
	
	WGPUSwapChain swapchain = wgpuDeviceCreateSwapChain(device, surface, &swapDesc);
	
	return swapchain;
}

WGPUTextureFormat webgpu::getSwapChainFormat(WGPUDevice /*device*/) {
	return WGPUTextureFormat_BGRA8Unorm;
}
