#pragma once

#include "il2cpp.h"
#include "il2cppp.h"

static const auto game_assembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");

template <typename t>
inline t read(uintptr_t addr) {
	return *reinterpret_cast<t*>(addr);
}

namespace cm
{
		
	inline auto get_player_team(CombatMaster_Battle_Gameplay_Player_PlayerRoot_o* plr) {
		return reinterpret_cast<int(__cdecl*)(CombatMaster_Battle_Gameplay_Player_PlayerRoot_o*)>(game_assembly + 0x4738EC0)(plr);
	}

	inline auto get_player_health(CombatMaster_Battle_Gameplay_Player_PlayerRoot_o* plr) {
		return reinterpret_cast<CombatMaster_Battle_Gameplay_Player_PlayerHealth_o * (__cdecl*)(CombatMaster_Battle_Gameplay_Player_PlayerRoot_o*)>(game_assembly + 0xAA0550)(plr);
	}

	inline auto is_dead(CombatMaster_Battle_Gameplay_Player_PlayerHealth_o* health) {
		return reinterpret_cast<bool(__cdecl*)(CombatMaster_Battle_Gameplay_Player_PlayerHealth_o*)>(game_assembly + 0x46C9260)(health);
	}

	inline auto get_mob_view(CombatMaster_Battle_Gameplay_Player_PlayerRoot_o* plr) {
		return reinterpret_cast<CombatMaster_Battle_Gameplay_Player_PlayerMobView_o * (__cdecl*)(CombatMaster_Battle_Gameplay_Player_PlayerRoot_o*)>(game_assembly + 0x4738880)(plr);
	}

	inline auto get_player_root_static_fields() -> uintptr_t {
		uintptr_t PlayerRoot_c = *reinterpret_cast<uintptr_t*>(game_assembly + 117592792);
		if (!PlayerRoot_c)
			return NULL;
		uintptr_t static_fields = *reinterpret_cast<uintptr_t*>(PlayerRoot_c + 0xB8);
		if (!static_fields)
			return NULL;
		return static_fields;
	}

	inline auto get_all_players() -> System_Collections_Generic_List_PlayerRoot__o* {
		auto static_fields = get_player_root_static_fields();
		if (!static_fields)
			return NULL;
		return read<System_Collections_Generic_List_PlayerRoot__o*>(static_fields + 0x18);
	}

	inline auto get_local_player() -> CombatMaster_Battle_Gameplay_Player_PlayerRoot_o* {
		auto static_fields = get_player_root_static_fields();
		if (!static_fields)
			return NULL;
		return read<CombatMaster_Battle_Gameplay_Player_PlayerRoot_o*>(static_fields + 0x8);
	}

}