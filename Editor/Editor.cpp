/*
Copyright(c) 2016-2018 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//= INCLUDES =======================
#include "Editor.h"
#include <memory>
#include "UI/ImGui_Implementation.h"
#include "UI/Widgets/Widget.h"
#include "Core/Context.h"
#include "UI/Widgets/MenuBar.h"
#include "UI/Widgets/Properties.h"
#include "UI/Widgets/Console.h"
#include "UI/Widgets/Hierarchy.h"
#include "UI/Widgets/AssetViewer.h"
#include "UI/Widgets/Viewport.h"
#include "UI/ThumbnailProvider.h"
#include "UI/EditorHelper.h"
#include "UI/Widgets/Toolbar.h"
//==================================

//= NAMESPACES ==========
using namespace std;
using namespace Directus;
//=======================

static SDL_Window* g_window = nullptr;

Editor::Editor()
{
	m_context = nullptr;
	m_widgets.emplace_back(make_unique<MenuBar>());
	m_widgets.emplace_back(make_unique<Toolbar>());
	m_widgets.emplace_back(make_unique<Properties>());
	m_widgets.emplace_back(make_unique<Console>());
	m_widgets.emplace_back(make_unique<Hierarchy>());
	m_widgets.emplace_back(make_unique<AssetViewer>());
	m_widgets.emplace_back(make_unique<Viewport>());
}

Editor::~Editor()
{
	Shutdown();
}

void Editor::Initialize(SDL_Window* window, Context* context)
{
	m_context = context;
	ThumbnailProvider::Get().Initialize(context);
	EditorHelper::Get().Initialize(context);

	g_window = window;
	ImGui_Impl_Initialize(g_window, context);
	ApplyStyle();

	for (auto& widget : m_widgets)
	{
		widget->Initialize(context);
	}
}

void Editor::HandleEvent(SDL_Event* event)
{
	ImGui_Impl_ProcessEvent(event);
}

void Editor::Update()
{	
	ImGui_Impl_NewFrame(g_window);
	DrawEditor();
	EditorHelper::Get().Update();
	ImGui::Render();
}

void Editor::Shutdown()
{
	m_widgets.clear();
	m_widgets.shrink_to_fit();
	ImGui_Impl_Shutdown();
}

void Editor::DrawEditor()
{
	for (auto& widget : m_widgets)
	{
		if (widget->GetIsWindow())
		{
			widget->Begin();
		}

		widget->Update();

		if (widget->GetIsWindow())
		{
			widget->End();
		}
	}
}

void Editor::ApplyStyle()
{
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	float fontSize				= 15.0f;
	float roundness				= 2.0f;
	ImVec4 white				= ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 text					= ImVec4(0.76f, 0.77f, 0.8f, 1.0f);
	ImVec4 backgroundDark		= ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
	ImVec4 backgroundMedium		= ImVec4(0.26f, 0.26f, 0.27f, 1.0f);
	ImVec4 backgroundLight		= ImVec4(0.37f, 0.38f, 0.39f, 1.0f);
	ImVec4 highlightBlue		= ImVec4(0.22f, 0.38f, 0.50f, 0.5f);
	ImVec4 highlightBlueHovered	= ImVec4(0.22f, 0.5f, 0.64f, 0.6f);
	ImVec4 highlightBlueActive	= ImVec4(0.22f, 0.5f, 0.8f, 0.7f);
	ImVec4 barBackground		= backgroundMedium;
	ImVec4 bar					= backgroundLight;
	ImVec4 barHovered			= ImVec4(0.411f, 0.411f, 0.411f, 1.0f);
	ImVec4 barActive			= ImVec4(0.509f, 0.509f, 0.509f, 1.0f);

	// Spatial
	style.WindowBorderSize		= 1.0f;
	style.FrameBorderSize		= 1.0f;
	//style.WindowMinSize		= ImVec2(160, 20);
	//style.FramePadding		= ImVec2(4, 2);
	//style.ItemSpacing			= ImVec2(6, 2);
	//style.ItemInnerSpacing	= ImVec2(6, 4);
	style.Alpha					= 1.0f;
	style.WindowRounding		= roundness;
	style.FrameRounding			= roundness;
	style.PopupRounding			= roundness;
	//style.IndentSpacing		= 6.0f;
	//style.ItemInnerSpacing	= ImVec2(2, 4);
	//style.ColumnsMinSpacing	= 50.0f;
	//style.GrabMinSize			= 14.0f;
	style.GrabRounding			= roundness;
	//style.ScrollbarSize		= 12.0f;
	style.ScrollbarRounding		= roundness;	

	// Colors
	style.Colors[ImGuiCol_Text]						= text;
	//style.Colors[ImGuiCol_TextDisabled]			= ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style.Colors[ImGuiCol_WindowBg]					= backgroundDark;
	//style.Colors[ImGuiCol_ChildBg]				= ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
	style.Colors[ImGuiCol_Border]					= backgroundLight;
	//style.Colors[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg]					= backgroundMedium;
	style.Colors[ImGuiCol_FrameBgHovered]			= highlightBlue;
	style.Colors[ImGuiCol_FrameBgActive]			= highlightBlueHovered;
	style.Colors[ImGuiCol_TitleBg]					= backgroundMedium;
	//style.Colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive]			= backgroundLight;
	style.Colors[ImGuiCol_MenuBarBg]				= backgroundMedium;
	style.Colors[ImGuiCol_ScrollbarBg]				= barBackground;
	style.Colors[ImGuiCol_ScrollbarGrab]			= bar;
	style.Colors[ImGuiCol_ScrollbarGrabHovered]		= barHovered;
	style.Colors[ImGuiCol_ScrollbarGrabActive]		= barActive;
	style.Colors[ImGuiCol_CheckMark]				= white;
	style.Colors[ImGuiCol_SliderGrab]				= bar;
	style.Colors[ImGuiCol_SliderGrabActive]			= barActive;
	style.Colors[ImGuiCol_Button]					= backgroundLight;
	style.Colors[ImGuiCol_ButtonHovered]			= highlightBlue;
	style.Colors[ImGuiCol_ButtonActive]				= highlightBlueHovered;
	style.Colors[ImGuiCol_Header]					= highlightBlue; // selected items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderHovered]			= highlightBlueHovered; // hovered items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderActive]				= highlightBlueActive;
	style.Colors[ImGuiCol_Separator]				= backgroundLight;
	//style.Colors[ImGuiCol_SeparatorHovered]		= ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	//style.Colors[ImGuiCol_SeparatorActive]		= ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip]				= backgroundMedium;
	style.Colors[ImGuiCol_ResizeGripHovered]		= highlightBlue;
	style.Colors[ImGuiCol_ResizeGripActive]			= highlightBlueHovered;
	style.Colors[ImGuiCol_CloseButton]				= backgroundLight;
	style.Colors[ImGuiCol_CloseButtonHovered]		= backgroundLight;
	style.Colors[ImGuiCol_CloseButtonActive]		= backgroundLight;
	//style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	//style.Colors[ImGuiCol_PlotLinesHovered]		= ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram]			= highlightBlue; // Also used for progress bar
	style.Colors[ImGuiCol_PlotHistogramHovered]		= highlightBlueHovered;
	style.Colors[ImGuiCol_TextSelectedBg]			= highlightBlue;
	style.Colors[ImGuiCol_PopupBg]					= backgroundMedium;
	style.Colors[ImGuiCol_DragDropTarget]			= backgroundLight;
	//style.Colors[ImGuiCol_ModalWindowDarkening]	= ImVec4(0.20f, 0.22f, 0.27f, 0.73f);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Standard Assets\\Editor\\CalibriLight.ttf", fontSize);
}