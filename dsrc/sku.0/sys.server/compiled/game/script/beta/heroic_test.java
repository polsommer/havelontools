package script.beta;

import script.*;
import script.library.*;

import java.util.HashSet;

import static script.library.holiday.closeOldWindow;

public class heroic_test extends base_script
{
    public heroic_test(){}
    public static final String HANDLER_SET_TOOL_CLASS = "handleSetToolClass";
    public static final string_id REDEEM= new string_id("sui", "redeem_set_token");
    public static final String[] SETS = {
            "Heroism Set",
            "Dire Fate Set",
            "Enforcer's Set",
            "Flawless Set",
            "Frontman Set",
            "Grenadier Set",
            "Juggernaut Set",
            "Dark Fury Set",
            "Guardian's Set",
            "Lightsaber Duelist's Set",
            "Blackbar's Doom Set",
            "First Responder's Set",
            "Striker's Set",
            "Dead Eye Set",
            "General's Set",
            "Hellstorm Set",
            "Gambler's Set",
            "Rogue Set",
            "Scoundrel's Set",
            "Assassins's Set",
            "The Ghost Set",
            "The Razor cat Set",
            "Tragedy Set",
            "Tinkers Set"
    };

    public int OnObjectMenuRequest(obj_id self, obj_id player, menu_info mi) throws InterruptedException
    {
        int menu = mi.addRootMenu(menu_info_types.ITEM_USE, REDEEM);

        return SCRIPT_CONTINUE;
    }
    public int OnObjectMenuSelect(obj_id self, obj_id player, int item) throws InterruptedException
    {
        startSetSelection(player);
        return SCRIPT_CONTINUE;
    }
    public void startSetSelection(obj_id player) throws InterruptedException
    {
        obj_id self = getSelf();
        String prompt = "Choose a Bio-Linked, No-Trade Set:";
        String title = "Set Redemption";
        int pid = sui.listbox(self, player, prompt, sui.OK_CANCEL, title, SETS, "handleOptionSelect", true, false);
        setWindowPid(player, pid);
    }
    public void setWindowPid(obj_id player, int pid) throws InterruptedException
    {
        if (pid > -1)
        {
            utils.setScriptVar(player, "character_builder.pid", pid);
        }
    }
    public int OnInitialize(obj_id self) throws InterruptedException
    {
        setObjVar(self, "noTradeShared", true);
        return SCRIPT_CONTINUE;
    }
    public int handleOptionSelect(obj_id self, dictionary params) throws InterruptedException {
        if (params == null || params.isEmpty()) {
            return SCRIPT_CONTINUE;
        }
        obj_id player = sui.getPlayerId(params);
        int btn = sui.getIntButtonPressed(params);
        int idx = sui.getListboxSelectedRow(params);
        if (btn == sui.BP_CANCEL) {
            cleanScriptVars(player);
            return SCRIPT_CONTINUE;
        }
        if (idx == -1 || idx > SETS.length) {
            cleanScriptVars(player);
            return SCRIPT_CONTINUE;
        }
       // sendSystemMessageTestingOnly(player, "Index Selected: "+idx);
        grantBioSet(player,idx,self);
        closeOldWindow(player);
        return SCRIPT_CONTINUE;
    }
    public void cleanScriptVars(obj_id player) throws InterruptedException
    {
        obj_id self = getSelf();
        utils.removeScriptVarTree(player, "character_builder");
        utils.removeScriptVarTree(self, "character_builder");
    }

    public static obj_id[] grantBioSet(obj_id player, int num, obj_id destroy_me) throws InterruptedException
    {
        obj_id pInv = utils.getInventoryContainer(player);
        HashSet theSet = new HashSet();
        String root = "";
        if (num == 0){
            root = "set_hero_01_01";
        }
        if (num == 1){
            root = "set_bh_utility_b_01_01";
        }
        if (num == 2){
            root = "set_bh_dps_01_01";
        }
        if (num == 3){
            root = "set_bh_utility_a_01_01";
        }
        if (num == 4){
            root = "set_commando_utility_a_01_01";
        }
        if (num == 5){
            root = "set_commando_dps_01_01";
        }
        if (num == 6){
            root = "set_commando_utility_b_01_01";
        }
        if (num == 7){
            root = "set_jedi_utility_a_01_01";
        }
        if (num == 8){
            root = "set_jedi_utility_b_01_01";
        }
        if (num == 9){
            root = "set_jedi_dps_01_01";
        }
        if (num == 10){
            root = "set_medic_utility_b_01_01";
        }
        if (num == 11){
            root = "set_medic_utility_a_01_01";
        }
        if (num == 12){
            root = "set_medic_dps_01_01";
        }
        if (num == 13){
            root = "set_officer_dps_01_01";
        }
        if (num == 14){
            root = "set_officer_utility_b_01_01";
        }
        if (num == 15){
            root = "set_officer_utility_a_01_01";
        }
        if (num == 16){
            root = "smuggler_utility_b_01_01";
        }
        if (num == 17){
            root = "set_smuggler_utility_a_01_01";
        }
        if (num == 18){
            root = "set_smuggler_dps_01_01";
        }
        if (num == 19){
            root = "set_spy_dps_01_01";
        }
        if (num == 20){
            root = "set_spy_utility_a_01_01";
        }
        if (num == 21){
            root = "set_spy_utility_b_01_01";
        }
        if (num == 22){
            root = "set_ent_01_01";
        }
        if (num == 23){
            root = "set_trader_01_01";
        }
        if (num>23){
            sendSystemMessageTestingOnly(player, "Index Selected: "+num+", Is out of bounds. Error. Try again.");
            return null;
        }


        theSet.add(static_item.createNewItemFunction("item_ring_"+root, pInv));
        theSet.add(static_item.createNewItemFunction("item_band_"+root, pInv));
        theSet.add(static_item.createNewItemFunction("item_necklace_"+root, pInv));
        theSet.add(static_item.createNewItemFunction("item_bracelet_r_"+root, pInv));
        theSet.add(static_item.createNewItemFunction("item_bracelet_l_"+root, pInv));
        obj_id[] items = new obj_id[theSet.size()];
        theSet.toArray(items);
        for (obj_id a : items){
            static_item.attachScript(a,"item.armor.biolink_item_non_faction");
            setObjVar(a, "noTrade", true);
        }
        showLootBox(player, items);
        static_item.destroyObject(destroy_me);
        return items;
    }

}
