#include "GlobalShader.h"
#include "CoreMinimal.h"
#include "DataDrivenShaderPlatformInfo.h"
#include "HAL/IConsoleManager.h"
#include "RHI.h"
#include "Shader.h"
#include "RenderUtils.h"
#include "SceneRendering.h"
#include "ScenePrivate.h"
#include "PostProcess/SceneRenderTargets.h"
#include "ProfilingDebugging/RealtimeGPUProfiler.h"
#include "SkyAtmosphereRendering.h"

class FCustomPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FCustomPS);
	SHADER_USE_PARAMETER_STRUCT(FCustomPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorTexture)
		SHADER_PARAMETER_SAMPLER(SamplerState, SceneColorTextureSampler)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()
};

IMPLEMENT_GLOBAL_SHADER(FCustomPS, "/Engine/Private/CustomShader.usf", "MainPS", SF_Pixel);
