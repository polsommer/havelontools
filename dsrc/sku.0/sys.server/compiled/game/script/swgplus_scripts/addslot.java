package script.swgplus_scripts;

import script.*;
import script.library.buff;
import script.library.static_item;

public class addslot extends base_script {

    public addslot() {
    }

    public int OnInitialize(obj_id self) throws InterruptedException
    {
        setObjVar(self, "noTradeShared", true);
        return SCRIPT_CONTINUE;
    }

    public int OnObjectMenuRequest(obj_id self, obj_id player, menu_info mi) throws InterruptedException {
        mi.addRootMenu(menu_info_types.ITEM_USE, new string_id("sui", "redeem_slot_token"));
        return SCRIPT_CONTINUE;
    }

    public int OnObjectMenuSelect(obj_id self, obj_id player, int item) throws InterruptedException {

        if(item == menu_info_types.ITEM_USE) {
            addJediSlot(player);
            playMusic(self, "sound/music_amb_underwater_b.snd");
            sendSystemMessageTestingOnly(player, "Congratulations, an additional character slot has been unlocked!");
            static_item.destroyObject(self);
            return SCRIPT_CONTINUE;
        }
        return SCRIPT_CONTINUE;
    }
}
