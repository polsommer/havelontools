package script.item;

import script.*;
import script.library.buff;

public class ent_buffer extends script.base_script {

    public ent_buffer() {
    }

    public int OnObjectMenuRequest(obj_id self, obj_id player, menu_info mi) throws InterruptedException {
        mi.addRootMenu(menu_info_types.ITEM_USE, new string_id("sui", "claim_ent_buff"));
        return SCRIPT_CONTINUE;
    }

    public int OnObjectMenuSelect(obj_id self, obj_id player, int item) throws InterruptedException {

        if(item == menu_info_types.ITEM_USE) {
            buff.applyBuff(player,"general_inspiration", 7200);
			buff.applyBuff(player,"artisan_inspiration", 7200);
			buff.applyBuff(player,"tailor_inspiration", 7200);
			buff.applyBuff(player,"droidengineer_inspiration", 7200);
			buff.applyBuff(player,"weaponsmith_inspiration", 7200);
			buff.applyBuff(player,"shipwright_inspiration", 7200);
			buff.applyBuff(player,"armorsmith_inspiration", 7200);
        }
        return SCRIPT_CONTINUE;
    }
}
