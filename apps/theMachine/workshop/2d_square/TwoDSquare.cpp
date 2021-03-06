﻿#include "TwoDSquare.h"

/*#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif */

TwoDSquare::TwoDSquare()
{
	mGDriver = NULL;
	mRightPanel = NULL;
	mScene = NULL;
}

TwoDSquare::TwoDSquare(CGraphicDriver *gdriver, wxPanel* panel)
{
	mGDriver = gdriver;
	mRightPanel = panel;
	mScene = NULL;
	loadGUI();
}

void 
TwoDSquare::init(CGraphicDriver *gdriver, wxPanel* panel)
{
	mGDriver = gdriver;
	mRightPanel = panel;
	setupGraphics();
	loadGUI();
}

TwoDSquare::~TwoDSquare()
{
	if (mScene != NULL)
	{
		delete mScene;
	}
	cleanGUI();
}

std::shared_ptr<CCamera>
TwoDSquare::getCam()
{
	return mCamPtr;
}

void 
TwoDSquare::setupGraphics()
{
	std::vector<std::string> splittedPath = strh::split(sys::getFullFilePath(), '\\');
	std::string projectPath;
	for (auto i : splittedPath)
	{
		if (i != "YEngine")
		{
			projectPath += i + "\\";
		}
		else
		{
			projectPath += i;
			break;
		}
	}

	std::string vsPath = projectPath + "//apps//theMachine//workshop//2d_square//gl//shader.vert";
	std::string fsPath = projectPath + "//apps//theMachine//workshop//2d_square/gl//shader.frag";


	mGDriver->createShader(vsPath, fsPath);
	
	mCam = CCamera(glm::vec3(0.0f, 0.0f, 5.0f),
				   glm::vec3(0.0f, 0.0f, 0.0f),
				   glm::vec3(0.0f, 1.0f, 0.0f));

	mCamPtr = std::make_shared<CCamera>(mCam);
	
	CMesh* mesh = new CMesh("quad");
	sVertex v0 = { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	sVertex v1 = { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 1.0) };
	sVertex v2 = { glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0, 1.0) };
	sVertex v3 = { glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0, 0.0) };
	mesh->mVertices = { v0, v1, v2,v3 };
	mesh->mIndices = { 0, 1, 2, 2, 3, 0 };

	mScene = new CScene(mGDriver);
	mScene->add(mesh);
	
	mUniformColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
	mGDriver->addUniform("custom_color", mUniformColor);

	mUniformColor2 = glm::vec4(0.0, 0.0, 1.0, 1.0);
	mGDriver->addUniform("custom_color_two", mUniformColor2);

	mUniformColorHasChanged = true;

	mGDriver->finalizeSetup();
}

void 
TwoDSquare::render()
{
	/*if (mUniformColorHasChanged)
	{
		mGDriver->updateUniform("custom_color", mUniformColor);
		mUniformColorHasChanged = false;
	}*/

	mScene->render(mGDriver->getShader());
}

void 
TwoDSquare::loadGUI()
{
	// http://zetcode.com/gui/wxwidgets/layoutmanagement/

	if (mRightPanel != NULL)
	{
		mRightPanel->DestroyChildren();
		mRightPanel->SetBackgroundColour(wxT("#ededed"));

		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *st1 = new wxStaticText(mRightPanel, wxID_ANY, wxT("Color uniform : "));
		hbox1->Add(st1, 0, wxRIGHT, 10);
		wxColourPickerCtrl* colourPickerCtrl = new wxColourPickerCtrl(mRightPanel, wxID_ANY, wxStockGDI::COLOUR_RED);
		colourPickerCtrl->Bind(	wxEVT_COLOURPICKER_CHANGED, 
								[&](wxColourPickerEvent& evt) 
								{
									mUniformColorHasChanged = true;
									auto newColor = evt.GetColour();
									mUniformColor.x = static_cast<float>(newColor.Red()) / 255.0f;
									mUniformColor.y = static_cast<float>(newColor.Green()) / 255.0f;
									mUniformColor.z = static_cast<float>(newColor.Blue()) / 255.0f;
									mUniformColor.w = 1.0f;
								});

		hbox1->Add(colourPickerCtrl, 2);
		vbox->Add(hbox1, 0, wxLEFT | wxRIGHT | wxTOP, 10);

		vbox->Add(-1, 25); // bottom space

		mRightPanel->SetSizer(vbox);
		mRightPanel->Layout();
	}
}

void 
TwoDSquare::cleanGUI()
{
	if (mRightPanel != NULL)
	{
		mRightPanel->DestroyChildren();
	}
}

RendererRegister<TwoDSquare> regSquare("TwoDSquare");