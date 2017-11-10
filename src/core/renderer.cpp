#include "main.h"

void VisualRender::Init()
{
	// Setup screen space
	mViewportFBO = new Framebuffers();
	mViewportFBO->Create(mWindow->width, mWindow->height);

	// Post-processing shaders
	mChromaticPostFX = new Framebuffers();
	mChromaticPostFX->Create(mWindow->width, mWindow->height);

	// Setup canvas
	if (iEnablePostProcessing)
		mViewportCanvas = new FramebufferCanvas(mChromaticPostFX);
	else
		mViewportCanvas = new FramebufferCanvas(mViewportFBO);

	// Setup mesh
	mChromaticMesh = new QuadMesh();

	// Load shaders
	mChromaticShaders = new PostFxShaders();
	mChromaticShaders->Load("fx_chromatic");

	// Setup Shadowmapping
	const int mShadowmapSize = 512;// 512;

	mShadowmapFBO = new Framebuffers(false, true);
	mShadowmapFBO->Create(mShadowmapSize, mShadowmapSize);

	mShadowmapCamera = new Camera();
	mShadowmapCamera->SetOrtho(-10, 10, -10, 10, -10, 20);
	mShadowmapCamera->SetLookAt(vec3(3.0f, 2.0f, 3.0f), vec3(0.0f), vec3(0, 1, 0));

	// Water Mesh
	const int mWaterFBODivider = 1;
	mWaterReflectionFBO = new Framebuffers(true, false);
	mWaterReflectionFBO->Create(mWindow->width / mWaterFBODivider, mWindow->height / mWaterFBODivider);
	mWaterRefractionFBO = new Framebuffers(true, true);
	mWaterRefractionFBO->Create(mWindow->width / mWaterFBODivider, mWindow->height / mWaterFBODivider);
	mWaterReflectionCam = new Camera();
	
	// Set variables
	mRenderingShadowFBO = false;
}

void VisualRender::Resized()
{
	// Resize framebuffer size
	mViewportFBO->Resize(mWindow->width, mWindow->height);
	mChromaticPostFX->Resize(mWindow->width, mWindow->height);
}

void VisualRender::Loop()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set default clear color
	glClearColor(152.0f / 255.0f, 186.0f / 255.0f, 242.0f / 255.0f, 1.0f);

	// Enable depth test & face culling
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// Store default camera
	mDefaultCamera = mCamera;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	mShadowmapFBO->Begin();
	mRenderingShadowFBO = true;

	// Set camera
	mCamera = mShadowmapCamera;

	// Render spatial scene
	mMain->Render();

	mRenderingShadowFBO = false;
	mShadowmapFBO->End();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Water rendering
	
	// Enable clipping
	glEnable(GL_CLIP_DISTANCE0);

	// Reflection
	mWaterReflectionFBO->Begin();
	mWaterReflectionCam->projection = mDefaultCamera->projection;
	mWaterReflectionCam->SetLookAt(mDefaultCamera->mOrigin * vec3(1, -1, 1), mDefaultCamera->mLookAt, vec3(0, 1, 0));
	
	mCamera = mWaterReflectionCam;
	mShadersMgr->mMaterial->mClippingPlane = vec4(0, 1, 0, 15);

	// Render scenes
	mMain->Render();
	
	mWaterReflectionFBO->End();

	///////////////////////////////////////////////////////////////////////////////////////

	// Reflection
	mWaterRefractionFBO->Begin();
	mCamera = mDefaultCamera;
	mShadersMgr->mMaterial->mClippingPlane = vec4(0, -1, 0, 15);

	// Render scenes
	mMain->Render();
	mWaterRefractionFBO->End();

	// Disable clipping
	glDisable(GL_CLIP_DISTANCE0);
	mShadersMgr->mMaterial->mClippingPlane = vec4(0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Restore the camera
	mCamera = mDefaultCamera;

	// Render to our framebuffer
	mViewportFBO->Begin();

	// Render spatial scene
	mMain->Render();

	// End rendering
	mViewportFBO->End();
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (iEnablePostProcessing)
	{
		// Post effect
		mChromaticPostFX->Begin();

		// Render post fx
		mChromaticShaders->Bind();
		mChromaticShaders->SetUniforms(mViewportFBO->renderTexID, mViewportFBO->depthTexID);
		mChromaticMesh->Draw();

		// End rendering
		mChromaticPostFX->End();
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Reset viewport
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, mWindow->width, mWindow->height);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Disable depth test & face culling
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render framebuffer texture
	mViewportCanvas->Draw(vec2(0, 0), vec2(mWindow->width, mWindow->height));

	// Draw the user interface
	mInterface->Redraw();
}

void VisualRender::Free()
{

}
