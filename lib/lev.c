#include <p24FJ128GB206.h>
#include <stdint.h>
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "i2c.h"
#include "timer.h"
#include "lcd.h"
#include "strm.h"
#include "lev.h"
#include "rng.h"
#include "cmd.h"


// Theme 0
char lab_general [101][17]={" ","Engine", "Gyrator", "Flame Thrower", "Delwelder", "Fibrillator", "Generator", "Dismantler", "Transponder", "Collider", "Transformer", "Controller", "Vaporizer", "Diffuser", "Amplifier", "Combustor", "Extruder", "Silencer", "Detonator", "Compressor", "Contraption", "Deflector", "Simulator", "Isomorph", "Defibulator ", "Accelerator", "Replicator", "Differential", "Sabilizer", "Indicator", "Defacer", "Distiller", "Elevator", "Engtangler", "Airlock", "Stopcock", "De-icer", "Mulcher", "Grindr", "Blinder", "Hoover", "Mincher", "Gedder", "Gamma Ray", "Engorger", "Jumper", "Spaceball", "Barge", "Breaks", "Thrusters", "Shunter", "Flywheel", "Flux Beam", "Extra Nipple", "Gamma Punch", "Pocket Bulb", "Death Slicer", "Lambda Flange", "U-Alert", "Warp Hoop", "Ultra Grill", "Eigen Crank", "Clutch Shifter", "Sub Network", "Lube Hose", "Ambiwing", "Beta-Pulsar", "Pico Plucker", "Psi Kick", "Radio Trellis", "Astro Mulcher", "Multiphaser", "Delta Hole", "Giga Bellows", "Cycloscope", "B-Fold", "Depth Synth", "Vibro Hitch", "Shift Baffle", "F-Whisker", "Extra Flush", "G-Spigot", "Gyro Peg", "Rear Frustrum", "O-Lube", "Vibro Ignition", "Orb Sphere", "Sub Tensor", "Cyclo Cube", "Lunar Canal", "Fibre Scrubber", "Terra Cover", "Theta Bypass", "Stun Rockets", "Grip Horn", "Technobraid", "P-Satellite", "Volt Puncher", "Giga Paddler", "Hypno Core", "Macro Slicer"};
// Theme 1
char lab_long [47][17]={" ","Spark Scrambler", "Clutch Catalyst", "Suction Scissor", "Epsilon Counter", "Photo Sprinkler", "Depth Sanitizer", "Lambda Lubricant", "Phono Structure", "Omicron Control", "Zeta-Ventricle ", "Flange Aperture", "Beta-Substrate", "Phase Collapser", "Pseudo Register", "Chroma Scratcher", "Spectro Scanner", "Delta Harmonizer", "Chlorosplitter", "Suction Compass", "Power Substrate", "Vibro Lubricant", "Nipple Membrane", "Plasma Sanitizer", "Psi Synthesizer", "Spectrum Conduit", "Kappa Ventricle", "Epsilon Shifter", "Iota Transporter", "Plectrotumbler", "Hydromechanism", "Chroma Irritant", "Omicron Crystal", "Fibre Sprinkler", "Mirror Manifold", "Spectro Capsule", "Wrench Dampener", "Power Trapezoid", "Ribbon Lubricant", "Plasma Solenoid", "Exlposion Sphere", "Flange Cartridge", "Mezzo-Restraint", "Photon Collapser", "Omicron Mulcher", "Synth Harmonizer", "Spine Scratcher"};
// Theme 2
char lab_homo [49][17]={" ","Eunuch's Wrench","Unix Wrench","Baud Tuner", "Bawdy Tuner", "Shea Butterer", "She Butterer", "Lie Detector", "Lye Detector", "Cocoa Eater", "Coco Eater", "Whale Tail", "Whale Tale", "Cheap Part", "Cheep Part", "Rap string", "Wrap string", "Beer Brew", "Beer Bruise", "Meet Peg", "Meat Peg", "Pea Pod", "Pee Pod", "Loot Booty", "Lute Booty", "Swtich sign", "Switch sine", "Wax Ceiling", "Wax Sealing", "Anti-Bertha", "Auntie Bertha", "Lip Bomb", "Lip Balm", "Planar Surface", "Planner surface", "Tacks Board", "Tax Board", "Elicit feelings", "Illicit feelings", "Gorilla War", "Guerilla War", "Happy Days", "Happy Daze", "Flee Horn", "Flea Horn", "Naval Treaty", "Navel Treaty", "Second Storey", "Second Story"};
// Theme 3
char lab_sym [47][17]={" ","~**%%", "*****", "$%%^@", ")*@$", "%%@%%&", "@$%%@%%", "**$$$", "!#$%%#", "#$^&@", ")%%^*(", "^&*@$", "_(#%%!#", "`<>:'@", "?????", "?#$%%!", "':[}=", "':<(*", "?|}+", "^$~@", "`,?}", "&%%@!", "-=&*", "//:\\", "><><>", "A##$$?", "&$^#", ")+-_#", "@#^$", "()_^^", "#$&*", "&&&&&", "()_^+", "!#%%", "&%%$#", "#####", "!<>{", "{$#%%", ":+*&", "<?#$", "|:{<", "$^@&", "<|}:{", "@$&!", "&+_%%#", "&#%%+", "|:))!#"};
// Theme 4
char lab_emo [47][17]={" ",":-(", "(*^.^*)", "(^_^)", "#^_^#", "n__ny", "(*^@^*)", "~~>_<~~", "^_~", "*o*  :}", ":P  *o*", "@_@", "<@_@>", "o_O", "^o^", "O_O", "-_-", "^_^", ":D  :\\", "*o*  (:", "8) *o*", ":}  :-{o", "X-D", ":-))", ">:[", ":-(", ":^*", ":\\ </3", "%%-}", "<*)))-{", "*<|:-)", "<3  :-)", "(>_<)", "((+_+))", "('_')", "(T_T)", "(=_=)", "~~~ ) \'>~", "(=^.^=)", "\\ (^o^) /", "(*_*)", ":-(  ;-)", ";-p :-3", ";-) :-###", ">^_^<", "(._.)", "~(_8^(|)"};
// Theme 5
char lab_num [47][17]={" ","734", "613", "709", "163", "666", "472", "920", "375", "649", "263", "255", "8573", "9384", "4568", "7345", "1256", "8763", "2345", "1432", "8546", "2366", "12453", "23561", "17357", "24627", "79425", "24564", "28654", "20492", "28935", "16346", "90646", "20492", "34626", "97643", "56674", "11264", "34789", "12345", "35670", "155362", "264473", "970024", "566374", "987774", "234617"};
// Theme 6
char lab_cons [47][17]={" ","mqhc","lyvj","dchy","kdfg","zqht","xzyl","chdn","pgmr","mwdf","mfcj","wjccx","wftnb","tgldt","zdqgt","mftgy","jklbf","rsxlk","lrytm","myfkz","sfwdr","hlwsg","jmfdf","xzxyl","lvfqx","lpkbb","jjxxn","ysdff","nfqfj","gyjys","jmvsw","ndgbtv","lgxfqk","kdhmpr","vjgdqp","dghxqd","xsxdjj","zylzvx","jbzbrs","jmkvqk","hyqwvk","gbzfsgs","tlhwwgj","tgpvgsk","nrvgvvb","gxgvhxl","bxphqdm"};
// Theme 7
char lab_adven [47][17]={" ","Nightosphere", "Prismos Pickles", "Hambo", "Prison of Shame", "MeeMow", "Treetrunks", "Bananna Guard", "King of Mars", "Treehouse", "BMO", "Angelface", "Glorb", "Mushroom", "Litch", "Simon", "Gunther No", "Neddy", "Fan Fiction", "Fries", "The Farm", "Canyon", "Grass Sword", "Susan Strong", "Science", "LSP", "The Maid", "Jaaames Baxter", "Destiny Gang", "Rainicorn", "Tiffany", "Graybles", "Party Pat", "Enchiridion", "Lemongrab", "Gumball Guardian", "Goliad", "Bacon Pancakes", "Dungeon Train", "Lumpy Space", "Super Porp", "Ax Bass", "Mind Games", "Demonic Eye", "Milk Suit", "Sentient Samwich", "Neptr"};
// Theme 8
char lab_wars [47][17]={" ","Deathstar", "X-Wing", "TIE Fighter", "Alderaan", "Coruscant", "Dagobah", "Endor", "Hoth", "Empire", "Jakku", "Rebel Alliance", "Naboo", "Starkiller", "Tatooine", "The Force", "Kylo Ren", "Rey", "Finn", "Skywalker", "Falcon", "Han Solo", "Yoda", "Princess Leia", "Jabba the Hut", "Wookies", "Ewoks", "Darth Vader", "Daddy Issues", "Darth Maul", "Palpatine", "Star Destroyer", "Landspeeder", "C-3PO", "BB-8", "R2-D2", "Lightsaber", "The Light Side", "The Dark Side", "Clone", "Chewbacca", "First Order", "It's a Trap!", "Hosian Prime", "POE", "Resistance", "AT-AT"};
// Theme 9
char lab_pir [47][17]={" ","Treasure", "Galleon", "Black Beard", "Carribean", "Tortuga", "Jack Sparrow", "Black Pearl", "Davvy Jones", "Islands", "Spiced Rum", "Rum Gone", "Starboard", "Port", "Mast", "Rigging", "Plank", "Keel Haul", "Anchor", "Bermuda", "Avast", "Shiver Timbers", "Monkey", "Poop Deck", "Cockswain", "Landlubber", "Chips Ahoy", "Batten Hatches", "Booty", "Crows Nest", "Jolly Roger", "Man-O-War", "Scallyway", "Calico Jack", "Captain Kidd", "Barbary Coast", "Cat O Nine", "Cutlass", "Cannon", "Powder Chest", "Kraken", "Parrot", "Wooden Leg", "Captian Hook", "Eye Patch", "Dread Pirate", "Hydra"};
// Theme 10
char lab_descr [18][17]={"Two Toggles C1","Green Button","Joystick","One Hot","Red C1","Word Wheel","Four Hot","Slider","Red C2","Four Green","Rocker","C2 Dial","Clutch","Dial C3","Red C3","Triangle","Two Toggles C3","Arming"};
int theme_len[11]={101,47,49,47,47,47,47,47,47,47,18};
int lab_len[3]={2,8,7};
int easy_lab[2]={0,1};
int med_lab[8]={0,1,2,4,5,7,8,9};
int hard_lab[7]={0,1,2,3,4,5,6};
char messages[10][33]={" ","Impending Didactic Aberration","They're over there, by their ship","Tr@n$|at0r M@lfun(#ion","Lol good luck (^_^)/* ttyl, bai","Can I Ha Your Numba","Thngs r brkng dwn","Come on, grab your friends!","May the force be with you","ARRG, spaceship now be a seaship"};
char instructions[3][33]={"Commands may be for your console","Shake ball to avoid asteroids","Press flip to escape wormwhole"};

_LEV level;

void __lev_init(void) {
	uint8_t i=0;
	uint8_t k;
	while(i<8){
		level.asteroids[i]=0;
		level.wormholes[i]=0;
		i++;
	}
	for (k = 0; k < 3; k++) { 
		while(i<6){
			level.label[k][i]=0;
			i++;
		}
	}
    level.lab_theme=0;
    level.mod=0;
    level.arg_freq=0;
    level.arg_shift=0;
    level.level_time=0;
    level.cmd_time=0;
    level.actuators=6; // We almost always use all of them
}

uint8_t __lev_already_exists(uint8_t *array, uint8_t array_size, uint8_t value) {
    uint8_t i;
    for (i = 0; i < array_size; i++)
        if (array[i] == value)
            return 1; // we found the same value, return true

    return 0; // if we get here we have not found it
}

void __lev_pickLabels(uint8_t theme){
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;
	uint8_t random_num;
	uint8_t random_num_array[18];

	for(j=0;j<18;j++){
		random_num = (uint8_t)rng_int(1,theme_len[theme]-1);
		while(__lev_already_exists(&random_num_array, 18, random_num)){
			random_num = (uint8_t)rng_int(1,theme_len[theme]-1);
        }
		random_num_array[j] = random_num;
	}
    led_on(&led3);

	while(i<6){
		level.label[0][i]=random_num_array[i];
		level.label[1][i]=random_num_array[i+6];
		level.label[2][i]=random_num_array[i+12];
        i++;
	}
    i = 0;
	
	switch(theme) { //FUCK THIS SHIT IN PARTICULAR 
		case 0:
			while(i<6){
				strcpy(level.label_name1[i],lab_general[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_general[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_general[level.label[2][i]]);
				i++;
		}
			break;
		case 1:
			while(i<6){
				strcpy(level.label_name1[i],lab_long[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_long[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_long[level.label[2][i]]);
				i++;
			}	
			break;
		case 2:
			while(i<6){
				strcpy(level.label_name1[i],lab_homo[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_homo[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_homo[level.label[2][i]]);
				i++;
			}	
			break;
		case 3:
			while(i<6){
				strcpy(level.label_name1[i],lab_sym[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_sym[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_sym[level.label[2][i]]);
				i++;
			}	
			break;
		case 4:
			while(i<6){
				strcpy(level.label_name1[i],lab_emo[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_emo[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_emo[level.label[2][i]]);
				i++;
			}	
			break;
		case 5:
			while(i<6){
				strcpy(level.label_name1[i],lab_num[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_num[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_num[level.label[2][i]]);
				i++;
			}	
			break;
		case 6:
			while(i<6){
				strcpy(level.label_name1[i],lab_cons[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_cons[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_cons[level.label[2][i]]);
				i++;
			}	
			break;
		case 7:
			while(i<6){
				strcpy(level.label_name1[i],lab_adven[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_adven[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_adven[level.label[2][i]]);
				i++;
			}	
			break;
		case 8:
			while(i<6){
				strcpy(level.label_name1[i],lab_wars[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_wars[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_wars[level.label[2][i]]);
				i++;
			}	
			break;
		case 9:
			while(i<6){
				strcpy(level.label_name1[i],lab_pir[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_pir[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_pir[level.label[2][i]]);
				i++;
			}	
            break;
		case 10:
			while(i<6){
                level.label[0][i]=i;
                level.label[1][i]=i+6;
                level.label[2][i]=i+12;
				strcpy(level.label_name1[i],lab_descr[level.label[0][i]]);
				strcpy(level.label_name2[i],lab_descr[level.label[1][i]]);
				strcpy(level.label_name3[i],lab_descr[level.label[2][i]]);
				i++;
			}	
			break;
		}	
}

void lev_setup(uint8_t lev_num){
    __lev_init();
	if (lev_num == 1){ // First level has no complications
		level.lab_theme=0;
		level.level_time=200;
		level.cmd_time=25;
		level.actuators=4; // Only use 4 buttons
		strcpy(level.message,instructions[0]);
		__lev_pickLabels(0);
	}
	else if (lev_num ==2){ // Second Level
		level.lab_theme=0;
		level.level_time=180;
		level.cmd_time=20;
		strcpy(level.message,instructions[0]);
		__lev_pickLabels(0);
	}
	else if (lev_num ==3){ // Third Level
		level.lab_theme=0;
		level.level_time=170;
		level.cmd_time=16;
		level.asteroids[0]=(uint8_t)rng_int(4,10); // Seed two asteroids
		level.asteroids[1]=(uint8_t)rng_int(12,20);

        strcpy(level.message,instructions[1]);
		__lev_pickLabels(0);
	}

	else if (lev_num ==4 ){ // Level 4 (tier 2)
		level.lab_theme=0;
		level.level_time=150;
		level.cmd_time=12;
		level.asteroids[0]=(uint8_t)rng_int(10,35); // Seed an asteroid
		level.wormholes[0]=(uint8_t)rng_int(0,5); // Seed two wormholes
		level.wormholes[1]=(uint8_t)rng_int(20,50);
		strcpy(level.message,instructions[2]);
		__lev_pickLabels(0);
	}

	else if (lev_num <7){ // Tier 2 levels
		uint8_t theme=(uint8_t)rng_coin_replace(rng_int(1,lab_len[0]),10,0); // 10% chance of default
		level.lab_theme=easy_lab[theme];
		level.level_time=140;
		level.cmd_time=11;
		level.asteroids[0]=(uint8_t)rng_int(0,15); // Seed two asteroids
		level.asteroids[1]=(uint8_t)rng_int(30,45);
		level.wormholes[0]=(uint8_t)rng_int(15,30);// Seed two wormholes
		level.wormholes[1]=(uint8_t)rng_int(30,45);
		strcpy(level.message,messages[theme]);
		__lev_pickLabels(theme);
		level.mod=(uint8_t)rng_coin_replace(rng_int(0,3),20,0);
		level.arg_freq=(uint8_t)rng_int(5,10);

	}

	else if (lev_num <10){ // Tier 3 levels
		uint8_t theme=(uint8_t)rng_coin_replace(rng_int(1,lab_len[1]),20,0);// 50% chance of default
		level.lab_theme=easy_lab[theme];
		level.level_time=120;
		level.cmd_time=10;
		level.asteroids[0]=(uint8_t)rng_int(0,10); // Seed three asteroids
		level.asteroids[1]=(uint8_t)rng_int(20,30);
		level.asteroids[2]=(uint8_t)rng_int(30,50);
		level.wormholes[0]=(uint8_t)rng_int(15,30);// Seed two wormholes
		level.wormholes[1]=(uint8_t)rng_int(30,45);
		strcpy(level.message,messages[theme]);
		__lev_pickLabels(theme);
		level.mod=(uint8_t)rng_coin_replace(rng_int(0,8),20,0);
		level.arg_shift=1; // Make them easy
		level.arg_freq=(uint8_t)rng_int(5,8);
	}

	else if (lev_num <14){ // Tier 4 levels
		uint8_t theme=(uint8_t)rng_coin_replace(rng_int(0,lab_len[1]),20,0);
		level.lab_theme=easy_lab[theme];
		level.level_time=100;
		level.cmd_time=9;
		level.asteroids[0]=(uint8_t)rng_int(0,10); // Seed three asteroids
		level.asteroids[1]=(uint8_t)rng_int(20,30);
		level.asteroids[2]=(uint8_t)rng_int(30,50);
		level.wormholes[0]=(uint8_t)rng_int(0,20);// Seed three wormholes
		level.wormholes[1]=(uint8_t)rng_int(20,30);
		level.wormholes[2]=(uint8_t)rng_int(30,45);
		strcpy(level.message,messages[theme]);
		__lev_pickLabels(theme);
		level.mod=(uint8_t)rng_coin_replace(rng_int(0,8),20,0);
		level.arg_shift=1; // Make them med
		level.arg_freq=(uint8_t)rng_int(4,6);
	}

	else if (lev_num <16){ // Tier 5 levels
		uint8_t theme=(uint8_t)rng_coin_replace(rng_int(0,lab_len[2]),20,0);
		level.lab_theme=easy_lab[theme];
		level.level_time=90;
		level.cmd_time=8;
		level.asteroids[0]=(uint8_t)rng_int(0,10); // Seed four asteroids
		level.asteroids[1]=(uint8_t)rng_int(20,30);
		level.asteroids[2]=(uint8_t)rng_int(30,40);
		level.asteroids[3]=(uint8_t)rng_int(40,50);
		level.wormholes[0]=(uint8_t)rng_int(0,10);// Seed four wormholes
		level.wormholes[1]=(uint8_t)rng_int(20,30);
		level.wormholes[2]=(uint8_t)rng_int(30,40);
		level.wormholes[3]=(uint8_t)rng_int(40,50);
		strcpy(level.message,messages[theme]);
		__lev_pickLabels(theme);
		level.mod=(uint8_t)rng_coin_replace(rng_int(0,8),10,0);
		level.arg_shift=3; // Make them hard
		level.arg_freq=rng_int(3,6);
	}
	else if (lev_num == 100){ // TEST LEVEL
		level.lab_theme=10;
		level.level_time=500;
		level.cmd_time=10;
		strcpy(level.message,messages[0]);
		__lev_pickLabels(10);
	}
	// lcd_print1(&lcdcmd[2],level.label_name[0][0]);

}


void lev_send(uint8_t console){
	//Send to appropriate Console:
	// - label#
	// - lab_theme
	// - mod
	// - arg_freq
	// - arg_shift
	if (console==1){

	}
	if (console==2){
		
	}
	if (console==3){
		
	}

	// For testing:

}

void lev_genCmd(void){
	uint16_t i, j, k;
	uint16_t cmd_num;

    for (k = 0; k < 3; k++) { // k for konsole
        for (i = 0; i < CONS_NUMACTS[k]-3; i++) { // for each actuator
            if (!CONS_HASREST[k][i]){
                cmd_num=cmd_get(k, i, 0);
                // strcpy(cmds[cmd_num].name,"Test Name");
                // printf("GROUPA1: %d\r\n", cmds[cmd_num].group);
                strcpy(cmds[cmd_num].name,lev_getName(k,cmds[cmd_num].group));
                
                cmd_str(cmd_num);

            }
            for (j = 1; j < CONS_STATES[k][i]; j++) {
                cmd_num=cmd_get(k, i, j);
                // strcpy(cmds[cmd_num].name,"Test Name");
                // printf("GROUPA2: %d\r\n", cmds[cmd_num].group);
                strcpy(cmds[cmd_num].name,lev_getName(k,cmds[cmd_num].group));
                
                cmd_str(cmd_num);
            }
        }
    }
}

char* lev_getName(uint8_t console, uint8_t group){
	// printf("GROUPB: %d\r\n", group);
	// printf("---\r\n");
	if (console == 0) {
		return level.label_name1[group];
	}
	if (console == 1) {
		return level.label_name2[group];
	}
	if (console == 2) {
		return level.label_name3[group];
	}
	// return "Test";
}

void lev_printCmd(uint8_t cons, uint8_t cmd_num){
	lcd_print(&lcd[cons],cmds[cmd_num].command);
}
