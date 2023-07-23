package script.theme_park.restuss_event;

import script.dictionary;
import script.library.ai_lib;
import script.library.static_item;
import script.library.utils;
import script.obj_id;

public class emperors_hand_loot extends script.base_script {
	public emperors_hand_loot() {
	}
	public int aiCorpsePrepared(obj_id self, dictionary params) throws InterruptedException {
		obj_id corpseInventory = utils.
		getInventoryContainer(self);
		if (corpseInventory == null) {
			return SCRIPT_CONTINUE;
		}
		if (!isIdValid(self)) {
			return SCRIPT_CONTINUE;
		}
		createMyLoot(self);
		return SCRIPT_CONTINUE;
	}
	public void createMyLoot(obj_id self) throws InterruptedException {
		obj_id corpseInventory = utils.
		getInventoryContainer(self);
		if (corpseInventory == null) {
			return;
		}
		String mobType = ai_lib.getCreatureName(self);
		if (mobType == null) {
			return;
		}
		int x = rand(1, 100);
		if (x < 5) { /* 100% Drop Chance: Legendary Loot Chest */
			static_item.createNewItemFunction("rare_loot_chest_quality_3", corpseInventory);
		}
		if (x < 100) { /* 100% Drop Chance: Jinsu Razor Lightsaber Schematic */
			static_item.createNewItemFunction("item_restuss_schematic_saber_04_01", corpseInventory);
		}
		if (x < 12) { /* 11% Drop Chance: Biological Focus Crystal */
			static_item.createNewItemFunction("item_force_crystal_04_01", corpseInventory);
		}
		if (x < 11) { /* 10% Drop Chance: Synapse Focus Crystal */
			static_item.createNewItemFunction("item_force_crystal_04_02", corpseInventory);
		}
		if (x < 10) { /* 9% Drop Chance: Concentrated Bacta Tank */
			static_item.createNewItemFunction("item_publish_gift_27_04_01", corpseInventory);
		}
		if (x < 6) { /* 8% Drop Chance: Jedi Meditation Room Deed */
			static_item.createNewItemFunction("item_tcg_loot_reward_series3_jedi_meditation_room_deed", corpseInventory);
		}
		if (x < 5) { /* 4% Drop Chance: Sith Meditation Room Deed */
			static_item.createNewItemFunction("item_tcg_loot_reward_series3_sith_meditation_room_deed", corpseInventory);
		}
		if (x < 7) { /* 6% Drop Chance: Sith Speeder Deed */
			static_item.createNewItemFunction("item_tcg_loot_reward_series1_sith_speeder", corpseInventory);
		}
		if (x < 9) { /* 8% Drop Chance: Sith Waistpack 4/5 */
			static_item.createNewItemFunction("item_collection_sith_holocron_01_04", corpseInventory);
		}
		if (x < 6) { /* 5% Drop Chance: Rare Rol Stone */
			static_item.createNewItemFunction("item_treasure_map_bonus_buff_04_01", corpseInventory);
		}
		if (x < 5) { /* 4% Drop Chance: Rare Nova Crystal */
			static_item.createNewItemFunction("item_treasure_map_bonus_buff_04_02", corpseInventory);
		}
		if (x < 4) { /* 3% Drop Chance: Rare Power Gem */
			static_item.createNewItemFunction("item_treasure_map_bonus_buff_04_03", corpseInventory);
		}
		if (x < 3) { /* 2% Drop Chance: Rare Corusca Gem */
			static_item.createNewItemFunction("item_treasure_map_bonus_buff_04_04", corpseInventory);
		}
		if (x < 2) { /* 1% Drop Chance: Rare Sasho Gem */
			static_item.createNewItemFunction("item_treasure_map_bonus_buff_04_05", corpseInventory);
		}
		if (x < 2) { /* 1% Drop Chance: Rare Ankarres Sapphyre */
			static_item.createNewItemFunction("item_treasure_map_bonus_buff_04_06", corpseInventory);
		}
		if (x < 1) { /* 0% Drop Chance: Maul's Vengeance Lightsaber Crystal */
			static_item.createNewItemFunction("item_color_crystal_02_12", corpseInventory);
		}
		if (x < 1) { /* 0% Drop Chance: Kun's Blood Lightsaber Crystal*/
			static_item.createNewItemFunction("item_color_crystal_02_13", corpseInventory);
		}
		/* String myLoot1 = "object/tangible/loot/loot_schematic/generic_limited_use_flashy.iff";
		createObject(myLoot1, corpseInventory, ""); */
		return;
	}
}
		