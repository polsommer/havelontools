package script.item;

import script.*;
import script.library.buff;

public class med_buffer extends script.base_script {

    public med_buffer() {
    }

    public int OnObjectMenuRequest(obj_id self, obj_id player, menu_info mi) throws InterruptedException {
        mi.addRootMenu(menu_info_types.ITEM_USE, new string_id("sui", "claim_med_buffs"));
        return SCRIPT_CONTINUE;
    }

    public int OnObjectMenuSelect(obj_id self, obj_id player, int item) throws InterruptedException {

        if(item == menu_info_types.ITEM_USE) {
			buff.applyBuff((player),"me_buff_strength_3", 7200);
			buff.applyBuff((player),"me_buff_health_2", 7200);
			buff.applyBuff((player),"me_buff_action_3", 7200);
			buff.applyBuff((player),"me_buff_precision_3", 7200);
			buff.applyBuff((player),"me_buff_agility_3", 7200);
			buff.applyBuff((player),"me_buff_melee_gb_1", 7200);
			buff.applyBuff((player),"me_buff_ranged_gb_1", 7200);
        }
        return SCRIPT_CONTINUE;
    }
}
