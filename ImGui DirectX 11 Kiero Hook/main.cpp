#define _CRT_SECURE_NO_WARNINGS
#include "includes.h"
#include "il2cpp.h"
#include "kiero/minhook/include/MinHook.h"
#include "il2cppp.h"
#include <iostream>
#include <algorithm>
#include "game.h"
#include "framework/framework.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_freetype.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace config
{

	bool no_sway = false;
	bool no_recoil = false;
	bool box_esp = true;
	bool no_spread = false;
	bool show_teammates = false;
	bool show_health = true;
	bool show_distance = true;
	bool show_name = true;
	bool aimbot = false;
	bool aimbot_smoothing = true;
	float smoothing = 2.f;
	bool draw_fov = true;
	float aim_fov = 100.f;
	bool instant_bolt_reload = true;
	int aim_hitbox = 0;

}

auto hack_window = c_window("cheat_window", ImVec2(485, 431 - 62), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

static int current_tab = 0;

void setup_aimbot_tab()
{

	static auto show_if_tab = []() -> bool {
		return current_tab == 0;
	};

	auto main = hack_window.insert_child("Main", ImVec2(15, (15 + 70 + 15 - 28) - 62), ImVec2(220, 248));
	{

		main->set_visibility_callback(show_if_tab);

		main->insert_element<c_checkbox>("enable_aimbot", "Aimbot", &config::aimbot);

		main->insert_element<c_combo>("aimbot_hitbox", "Target hitbox", &config::aim_hitbox, std::vector<std::string>{ "Head", "Chest", "Nearest" });

		main->insert_element<c_checkbox>("enable_smoothing", "Smoothing", &config::aimbot_smoothing);

		main->insert_element<c_slider_float>("aimbot_smoothness", "Smoothness", &config::smoothing, 0.f, 100.f, "%.1f");

		main->insert_element<c_checkbox>("draw_aimbot_fov", "Draw FOV", &config::draw_fov);

		main->insert_element<c_slider_float>("aimbot_fov", "Aimbot FOV", &config::aim_fov, 0.f, 600.f);

	}

	auto other = hack_window.insert_child("Other", ImVec2(15 + 220 + 15, (15 + 70 + 15 - 28) - 62), ImVec2(220, 248));
	{

		other->insert_element<c_label>("soon_tm_label", "Coming soon...");

		other->set_visibility_callback(show_if_tab);

	}

}

void setup_visuals_tab()
{

	static auto show_if_tab = []() -> bool {
		return current_tab == 1;
	};

	auto main = hack_window.insert_child("Players", ImVec2(15, (15 + 70 + 15 - 28) - 62), ImVec2(220, 248));
	{

		main->insert_element<c_checkbox>("enable_box_esp", "Box ESP", &config::box_esp);

		main->insert_element<c_checkbox>("enable_distance_esp", "Distance ESP", &config::show_distance);

		main->insert_element<c_checkbox>("enable_name_esp", "Name ESP", &config::show_name);

		main->insert_element<c_checkbox>("show_health_bar", "Health bar", &config::show_health);

		main->insert_element<c_checkbox>("show_teammates", "Show teammates", &config::show_teammates);

		main->set_visibility_callback(show_if_tab);

	}

	auto other = hack_window.insert_child("World", ImVec2(15 + 220 + 15, (15 + 70 + 15 - 28) - 62), ImVec2(220, 248));
	{

		other->set_visibility_callback(show_if_tab);

	}

}

void setup_misc_tab()
{

	static auto show_if_tab = []() -> bool {
		return current_tab == 3;
	};

	auto main = hack_window.insert_child("Legit modifications", ImVec2(15, (15 + 70 + 15 - 28) - 62), ImVec2(220, 248));
	{

		main->insert_element<c_checkbox>("enable_no_sway", "No sway", &config::no_sway);

		main->insert_element<c_checkbox>("enable_no_recoil", "No recoil", &config::no_recoil);

		main->insert_element<c_checkbox>("enable_no_spread", "No spread", &config::no_spread);

		main->set_visibility_callback(show_if_tab);

	}

	auto other = hack_window.insert_child("Other", ImVec2(15 + 220 + 15, (15 + 70 + 15 - 28) - 62), ImVec2(220, 248));
	{

		other->set_visibility_callback(show_if_tab);

	}

}

void setup_ui()
{

	setup_aimbot_tab();

	setup_visuals_tab();

	setup_misc_tab();

	hack_window.insert_element<c_label_divider>("bottom_divider", "##tabs");

	hack_window.insert_element<c_tab>("Aimbot", ICON_FA_CROSSHAIRS, ImVec2(160 - 10, 70), 0, &current_tab, true);

	hack_window.insert_element<c_tab>("Visuals", ICON_FA_ARROWS_TO_EYE, ImVec2(160, 70), 1, &current_tab, true)->set_same_line(true);

	hack_window.insert_element<c_tab>("Miscellaneous", ICON_FA_GEARS, ImVec2(160 - 12, 70), 3, &current_tab, true)->set_same_line(true);

}

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

uintptr_t original_weapon_sway_update{ NULL };
uintptr_t original_set_target_recoil{ NULL };
uintptr_t original_bullet_start_move{ NULL };

void __cdecl hk_weapon_sway_update(void* thisptr) {
	if (config::no_sway)
		return;
	else
		return reinterpret_cast<decltype(&hk_weapon_sway_update)>(original_weapon_sway_update)(thisptr);
}

void __cdecl hk_set_target_recoil(void* thisptr, float recoil) {
	if (config::no_recoil)
		return reinterpret_cast<decltype(&hk_set_target_recoil)>(original_set_target_recoil)(thisptr, 0.f);
	else
		return reinterpret_cast<decltype(&hk_set_target_recoil)>(original_set_target_recoil)(thisptr, recoil);
}

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig tmp_cfg{};
	tmp_cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_Bold;
	auto one = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 13.f, &tmp_cfg);

	float baseFontSize = 26.f; // 13.0f is the size of the default font. Change to the font size you use.
	float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, ICON_MAX_FA };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.GlyphMinAdvanceX = iconFontSize;
	icons_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_Bold;
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
	setup_ui();
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

inline void DrawBox(ImVec2 pos, ImVec2 size, ImColor color)
{
	const ImRect rect_bb(pos, pos + size);

	ImGui::GetBackgroundDrawList()->AddRect(rect_bb.Min - ImVec2(1, 1), rect_bb.Max + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, color.Value.w));
	ImGui::GetBackgroundDrawList()->AddRect(rect_bb.Min + ImVec2(1, 1), rect_bb.Max - ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, color.Value.w));
	ImGui::GetBackgroundDrawList()->AddRect(rect_bb.Min, rect_bb.Max, color);
}

bool show_menu = true;
bool init = false;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	
	if (!init)
	{

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			hack_window.set_visibility_callback([]() -> bool {
				return show_menu;
				});
			init = true;
		}
		else
			return oPresent(pSwapChain, SyncInterval, Flags);

	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_F4) & 1 || GetAsyncKeyState(VK_INSERT) & 1)
		show_menu = !show_menu;

	hack_window.render();

	if (cm::get_all_players() && cm::get_local_player())
	{

		auto all_players = cm::get_all_players();
		auto local_player = cm::get_local_player();

		for (int i = 0; i < all_players->fields._size; i++)
		{

			auto player = all_players->fields._items->m_Items[i];

			if (!player || player == local_player)
				continue;

			const auto player_health = cm::get_player_health(player);

			if (cm::is_dead(player_health))
				continue; // na he done died...

			const bool teammate = cm::get_player_team(player) == cm::get_player_team(local_player);

			if (teammate && !config::show_teammates)
				continue;

			const auto mob_view = cm::get_mob_view(player);

			ImVec2 head_screen{}, foot_screen{};

			auto head_pos = il2cpp::get_transform_position(player->fields._firstPersonPivot);
			auto foot_pos = head_pos; foot_pos.y -= 1.4f;

			if (!il2cpp::world_to_screen(head_pos, head_screen) || !il2cpp::world_to_screen(foot_pos, foot_screen))
				continue;

			int height = head_screen.y - foot_screen.y;
			int width = height / 4;

			int BoxX;
			int BoxY;
			int BoxWidth;
			int BoxHeight;
			float Entity_x = foot_screen.x - width;
			float Entity_y = foot_screen.y;
			float Entity_w = height / 2;
			BoxX = Entity_x;
			BoxY = Entity_y;
			BoxWidth = Entity_w;
			BoxHeight = height;

			if (config::show_distance)
			{

				const float distance = head_pos.distance(il2cpp::get_transform_position(local_player->fields._firstPersonPivot));

				char tmp[40];
				sprintf_s(tmp, "%.1f", distance);

				ImGui::GetForegroundDrawList()->AddText(ImVec2(BoxX + ((BoxWidth - ImGui::CalcTextSize(std::string(std::string(tmp) + "M").c_str()).x) / 2.f), BoxY + 2.f), IM_COL32_WHITE, std::string(std::string(tmp) + "M").c_str());

			}

			if (config::box_esp)
				DrawBox(ImVec2(BoxX, BoxY), ImVec2(BoxWidth, BoxHeight), teammate ? ImColor(125, 205, 255, 255) : ImColor(252, 245, 104, 255));

		}

	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);

}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			MH_CreateHook((uintptr_t*)(game_assembly + 0x3F5FE70), hk_weapon_sway_update, (LPVOID*)&original_weapon_sway_update);
			MH_CreateHook((uintptr_t*)(game_assembly + 0x46D5050), hk_set_target_recoil, (LPVOID*)&original_set_target_recoil);

			if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
				std::cout << "no work\n";
				MessageBoxA(NULL, "err 1", "err 1", NULL);
				exit(0);
			}
			init_hook = true;
#ifdef _DEBUG
			AllocConsole();

			if (!freopen(("CONOUT$"), ("w"), stdout))
			{
				FreeConsole();
				return NULL;
			}
#endif

			if (!il2cpp::sanity())
			{
				std::cout << "no work (2)\n";
				MessageBoxA(NULL, "err 2", "err 2", NULL);
				exit(0);
			}

#ifdef _DEBUG
			std::cout << "inited\n";

			std::cout << "og_sway: " << std::hex << original_weapon_sway_update << "\n";
			std::cout << "og_target_recoil: " << std::hex << original_set_target_recoil << "\n";
#endif

		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}