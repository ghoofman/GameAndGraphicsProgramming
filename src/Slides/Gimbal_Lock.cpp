#include "Gimbal_Lock.h"
#include "../Main.h"

#ifdef ADDON_imgui
#include "OPimgui.h"
#endif

void GimbalLock::Init()
{
	scene.Init(16);

	arrow.Init("person.opm");
	ringX.Init("ring.opm");
	ringY.Init("ring.opm");
	ringZ.Init("ring.opm");
	ringGrey.Init("ring.opm");

	effect = OPeffectGen(
		"Common/Blinn.vert",
		"Common/BlinnColor.frag",
		OPATTR_POSITION | OPATTR_NORMAL,
		"Model Effect",
		ringX.mesh->vertexLayout.stride);

	effectTextured = OPeffectGen(
		"Common/Texture3D.vert",
		"Common/Texture.frag",
		OPATTR_POSITION | OPATTR_NORMAL | OPATTR_UV,
		"Textured Effect",
		arrow.mesh->vertexLayout.stride);

	colorRed = OPvec4(1, 0, 0, 1);
	colorGreen = OPvec4(1, 1, 0, 1);
	colorBlue = OPvec4(0, 0, 1, 1);
	colorGrey = OPvec4(0.7, 0.7, 0.7, 1);

	materialRed.Init(&effect);
	materialRed.AddParam("uColor", &colorRed);

	materialGreen.Init(&effect);
	materialGreen.AddParam("uColor", &colorGreen);

	materialBlue.Init(&effect);
	materialBlue.AddParam("uColor", &colorBlue);

	materialGrey.Init(&effect);
	materialGrey.AddParam("uColor", &colorGrey);

	texture = (OPtexture*)OPcmanLoadGet("person.png");
	materialColored.Init(&effectTextured);
	materialColored.AddParam("uColorTexture", texture);

	scene.Add(&ringX, &materialRed);
	scene.Add(&ringY, &materialGreen);
	scene.Add(&ringZ, &materialBlue);
	scene.Add(&arrow, &materialColored);

	rotation = OPvec3(0);
	rotationTarget = OPvec3(0);

	OPcmanLoad("Ubuntu.opf");

	OPfontSystemLoadEffects();
	FontManager = OPfontManagerSetup("Ubuntu.opf", NULL, 0);
	FontManager->_align = OPfontAlign::OPFONT_ALIGN_CENTER;
	FontManager->scale = 0.5;

}

OPint GimbalLock::Update(OPtimer * timer)
{

	f32 rotX = rotation.x * OPpi / 180.0;
	f32 rotY = rotation.y * OPpi / 180.0;
	f32 rotZ = rotation.z * OPpi / 180.0;

	if (interpolateRotation) {
		rotationTime += timer->Elapsed;
		OPfloat perc = rotationTime / 3000.0f;
		if (perc > 1.0) {
			interpolateRotation = 0;
			perc = 1.0;
			rotation = rotationTarget;
		}

		rotX = (rotation.x + ((rotationTarget.x - rotation.x) * perc)) * OPpi / 180.0;
		rotY = (rotation.y + ((rotationTarget.y - rotation.y) * perc)) * OPpi / 180.0;
		rotZ = (rotation.z + ((rotationTarget.z - rotation.z) * perc)) * OPpi / 180.0;
	}
	// Initial States

	ringX.world = OPMAT4_IDENTITY;
	ringY.world = OPMAT4_IDENTITY;
	ringZ.world = OPMAT4_IDENTITY;
	arrow.world = OPMAT4_IDENTITY;


	// ringY
	ringX.world.RotY(rotY);
	ringZ.world.RotY(rotY)->RotX(rotX);

	// Add rotations
	ringX.world.RotX(rotX);
	ringY.world.RotY(rotY);
	ringZ.world.RotZ(rotZ);


	// Rescale
	ringX.world.Scl(2.4)->RotX(-OPpi_2)->RotZ(OPpi_2);
	ringY.world.Scl(3.0);
	ringZ.world.Scl(2.0)->RotY(OPpi_2)->RotX(-OPpi_2)->RotZ(OPpi_2);


	//arrow.world.RotY(rotation.y)->RotX(rotation.x)->RotZ(rotation.z);
	arrow.world.RotY(rotY)->RotX(rotX)->RotZ(rotZ)->Scl(0.5);


	ringGrey.world = OPMAT4_IDENTITY;
	ringGrey.world.RotZ(OPpi_2)->RotZ(rotationGrey * OPpi / 180.0);
	ringGrey.world.Scl(1.8)->RotX(-OPpi_2);

	scene.camera.Update();

	return scene.Update(timer);
}


void GimbalLock::RenderMain(OPfloat delta) {
	scene.Render(delta);

	FontManager->_color = OPvec4(1, 1, 0, 1);
	OPfontRenderBegin(FontManager);
	OPfontRender("Y", OPvec2(OPRENDER_WIDTH / 2 - 20, 10));
	OPfontRenderEnd();

	FontManager->_color = OPvec4(1, 0, 0, 1);
	OPfontRenderBegin(FontManager);
	OPfontRender("X", OPvec2(OPRENDER_WIDTH / 2, 10));
	OPfontRenderEnd();

	FontManager->_color = OPvec4(0, 0, 1, 1);
	OPfontRenderBegin(FontManager);
	OPfontRender("Z", OPvec2(OPRENDER_WIDTH / 2 + 20, 10));
	OPfontRenderEnd();

}

void GimbalLock::RenderSecondary() {
#if RUN_SECONDARY_WINDOW
	SecondaryWindow->Bind();
#endif

#ifdef ADDON_imgui
	OPimguiNewFrame();
	{
		ImGui::Text("Gimbal Lock");
		ImGui::DragFloat3("Rotation", (float*)&rotation, 1.0f, -180, 180);
		//ImGui::DragFloat3("Camera", (float*)&scene.camera.pos, 0.25f, -10.0, 10.0f);
		if (ImGui::Button("Start")) {
			interpolateRotation = 1;
			rotationTime = 0;
			rotationTarget = OPvec3(90, 0, 90);
		}
		if (ImGui::Button("Show Grey")) {
			scene.Add(&ringGrey, &materialGrey);
		}
		if (ImGui::Button("Remove Grey")) {
			scene.index--;
		}
		ImGui::DragFloat("Rotation Grey", (float*)&rotationGrey, 1.0f, -180, 180);
		if (ImGui::Button("Interpolate")) {
			interpolateRotation = 1;
			rotationTime = 0;
			rotationTarget = OPvec3(180, 90, 180);
		}
	}
#endif

#if RUN_SECONDARY_WINDOW
	OPrenderClear(0.2, 0.2, 0.2);
#endif

#ifdef ADDON_imgui
	ImGui::Render();
#endif
}

void GimbalLock::Render(OPfloat delta)
{
#if RUN_SECONDARY_WINDOW
	MainWindow->Bind();
#endif
	OPrenderClear(0, 0, 0);

	RenderMain(delta);

#if RUN_SECONDARY_WINDOW
	// If there's a secondary window, present the Main Window first
	// Otherwise we're rendering to the second window
	OPrenderPresent();
#endif

	RenderSecondary();
	OPrenderPresent();
}

GimbalLock gimbalLock;
void GimbalkLockInit(OPgameState* last) { gimbalLock.Init(); }
OPint GimbalkLockUpdate(OPtimer* time) { return gimbalLock.Update(time); }
void GimbalkLockRender(OPfloat delta) { gimbalLock.Render(delta); }

OPgameState GIMBAL_LOCK = {
	GimbalkLockInit,
	GimbalkLockUpdate,
	GimbalkLockRender,
	NULL
};
