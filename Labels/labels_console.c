#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "i2c.h"
#include "timer.h"
#include "lcd.h"
#include "strmanip.h"
#include "labels_console.h"


// Theme 0
char lab_general [100][17]={"Engine", "Gyrator", "Flame Thrower", "Delwelder", "Fibrillator", "Generator", "Dismantler", "Transponder", "Collider", "Transformer", "Controller", "Vaporizer", "Diffuser", "Amplifier", "Combustor", "Extruder", "Silencer", "Detonator", "Compressor", "Contraption", "Deflector", "Simulator", "Isomorph", "Defibulator ", "Accelerator", "Replicator", "Differential", "Sabilizer", "Indicator", "Defacer", "Distiller", "Elevator", "Engtangler", "Airlock", "Stopcock", "De-icer", "Mulcher", "Grindr", "Blinder", "Hoover", "Mincher", "Gedder", "Gamma Ray", "Engorger", "Jumper", "Spaceball", "Barge", "Breaks", "Thrusters", "Shunter", "Flywheel", "Flux Beam", "Extra Nipple", "Gamma Punch", "Pocket Bulb", "Death Slicer", "Lambda Flange", "U-Alert", "Warp Hoop", "Ultra Grill", "Eigen Crank", "Clutch Shifter", "Sub Network", "Lube Hose", "Ambiwing", "Beta-Pulsar", "Pico Plucker", "Psi Kick", "Radio Trellis", "Astro Mulcher", "Multiphaser", "Delta Hole", "Giga Bellows", "Cycloscope", "B-Fold", "Depth Synth", "Vibro Hitch", "Shift Baffle", "F-Whisker", "Extra Flush", "G-Spigot", "Gyro Peg", "Rear Frustrum", "O-Lube", "Vibro Ignition", "Orb Sphere", "Sub Tensor", "Cyclo Cube", "Lunar Canal", "Fibre Scrubber", "Terra Cover", "Theta Bypass", "Stun Rockets", "Grip Horn", "Technobraid", "P-Satellite", "Volt Puncher", "Giga Paddler", "Hypno Core", "Macro Slicer"};
// Theme 1
char lab_long [46][17]={"Spark Scrambler", "Clutch Catalyst", "Suction Scissor", "Epsilon Counter", "Photo Sprinkler", "Depth Sanitizer", "Lambda Lubricant", "Phono Structure", "Omicron Control", "Zeta-Ventricle ", "Flange Aperture", "Beta-Substrate", "Phase Collapser", "Pseudo Register", "Chroma Scratcher", "Spectro Scanner", "Delta Harmonizer", "Chlorosplitter", "Suction Compass", "Power Substrate", "Vibro Lubricant", "Nipple Membrane", "Plasma Sanitizer", "Psi Synthesizer", "Spectrum Conduit", "Kappa Ventricle", "Epsilon Shifter", "Iota Transporter", "Plectrotumbler", "Hydromechanism", "Chroma Irritant", "Omicron Crystal", "Fibre Sprinkler", "Mirror Manifold", "Spectro Capsule", "Wrench Dampener", "Power Trapezoid", "Ribbon Lubricant", "Plasma Solenoid", "Exlposion Sphere", "Flange Cartridge", "Mezzo-Restraint", "Photon Collapser", "Omicron Mulcher", "Synth Harmonizer", "Spine Scratcher"};
// Theme 2
char lab_homo [46][17]={"Baud Tuner", "Bawdy Tuner", "Shea Butterer", "She Butterer", "Lie Detector", "Lye Detector", "Cocoa Eater", "Coco Eater", "Whale Tail", "Whale Tale", "Cheap Part", "Cheep Part", "Rap string", "Wrap string", "Beer Brew", "Beer Bruise", "Meet Peg", "Meat Peg", "Pea Pod", "Pee Pod", "Loot Booty", "Lute Booty", "Swtich sign", "Switch sine", "Wax Ceiling", "Wax Sealing", "Anti-Bertha", "Auntie Bertha", "Lip Bomb", "Lip Balm", "Planar Surface", "Planner surface", "Tacks Board", "Tax Board", "Elicit feelings", "Illicit feelings", "Gorilla War", "Guerilla War", "Happy Days", "Happy Daze", "Flee Horn", "Flea Horn", "Naval Treaty", "Navel Treaty", "Second Storey", "Second Story"};
// Theme 3
char lab_sym [46][17]={"~**%%", "*****", "$%%^@", ")*@$", "%%@%%&", "@$%%@%%", "**$$$", "!#$%%#", "#$^&@", ")%%^*(", "^&*@$", "_(#%%!#", "`<>:'@", "?????", "?#$%%!", "':[}=", "':<(*", "?|}+", "^$~@", "`,?}", "&%%@!", "-=&*", "//:\\", "><><>", "A##$$?", "&$^#", ")+-_#", "@#^$", "()_^^", "#$&*", "&&&&&", "()_^+", "!#%%", "&%%$#", "#####", "!<>{", "{$#%%", ":+*&", "<?#$", "|:{<", "$^@&", "<|}:{", "@$&!", "&+_%%#", "&#%%+", "|:))!#"};
// Theme 4
char lab_emo [46][17]={":-(", "(*^.^*)", "(^_^)", "#^_^#", "n__ny", "(*^@^*)", "~~>_<~~", "^_~", "*o*  :}", ":P  *o*", "@_@", "<@_@>", "o_O", "^o^", "O_O", "-_-", "^_^", ":D  :\\", "*o*  (:", "8) *o*", ":}  :-{o", "X-D", ":-))", ">:[", ":-(", ":^*", ":\\ </3", "%-}", "<*)))-{", "*<|:-)", "<3  :-)", "(>_<)", "((+_+))", "('_')", "(T_T)", "(=_=)", "~~~ ) \'>~", "(=^.^=)", "\\ (^o^) /", "(*_*)", ":-(  ;-)", ";-p :-3", ";-) :-###", ">^_^<", "(._.)", "~(_8^(|)"};
// Theme 5
char lab_num [46][17]={"734", "613", "709", "163", "666", "472", "920", "375", "649", "263", "255", "8573", "9384", "4568", "7345", "1256", "8763", "2345", "1432", "8546", "2366", "12453", "23561", "17357", "24627", "79425", "24564", "28654", "20492", "28935", "16346", "90646", "20492", "34626", "97643", "56674", "11264", "34789", "12345", "35670", "155362", "264473", "970024", "566374", "987774", "234617"};
// Theme 6
char lab_cons [46][17]={"mqhc","lyvj","dchy","kdfg","zqht","xzyl","chdn","pgmr","mwdf","mfcj","wjccx","wftnb","tgldt","zdqgt","mftgy","jklbf","rsxlk","lrytm","myfkz","sfwdr","hlwsg","jmfdf","xzxyl","lvfqx","lpkbb","jjxxn","ysdff","nfqfj","gyjys","jmvsw","ndgbtv","lgxfqk","kdhmpr","vjgdqp","dghxqd","xsxdjj","zylzvx","jbzbrs","jmkvqk","hyqwvk","gbzfsgs","tlhwwgj","tgpvgsk","nrvgvvb","gxgvhxl","bxphqdm"};
// Theme 7
char lab_adven [46][17]={"Nightosphere", "Prismos Pickles", "Hambo", "Shame Prison", "MeeMow", "Treetrunks", "Bananna Guard", "Peppermint Butler", "Treehouse", "BMO", "Angelface", "Glorb", "Mushroom", "Litch", "Simon", "Gunther No", "Neddy", "Fan Fiction", "Fries", "The Farm", "Canyon", "Grass Sword", "Susan Strong", "Science", "LSP", "The Maid", "James Baxter", "Destiny Gang", "Ranicorn", "Tiffany", "Graybles", "Party Pat", "Enchiridion", "Lemongrab", "Gumball Guardian", "Goliad", "Bacon Pancakes", "Dungeon Train", "Lumpy Space", "Super Porp", "Ax Bass", "Mind Games", "Demonic Eye", "Milk Suit", "Sentient Sandwich", "Neptr"};
// Theme 8
char lab_wars [46][17]={"Deathstar", "X-Wing", "TIE Fighter", "Alderaan", "Coruscant", "Dagobah", "Endor", "Hoth", "Empire", "Jakku", "Rebel Alliance", "Naboo", "Starkiller", "Tatooine", "The Force", "Kylo Ren", "Rey", "Finn", "Skywalker", "Falcon", "Han Solo", "Yoda", "Princess Leia", "Jabba the Hut", "Wookies", "Ewoks", "Darth Vader", "Daddy Issues", "Darth Maul", "Palpatine", "Star Destroyer", "Landspeeder", "C-3PO", "BB-8", "R2-D2", "Lightsaber", "The Light Side", "The Dark Side", "Clone", "Chewbacca", "First Order", "It's a Trap!", "Hosian Prime", "POE", "Resistance", "AT-AT"};
// Theme 9
char lab_pir [46][17]={"Treasure", "Galleon", "Black Beard", "Carribean", "Tortuga", "Jack Sparrow", "Black Pearl", "Davvy Jones", "Islands", "Spiced Rum", "Rum Gone", "Starboard", "Port", "Mast", "Rigging", "Plank", "Keel Haul", "Anchor", "Bermuda", "Avast", "Shiver Timbers", "Monkey", "Poop Deck", "Cockswain", "Landlubber", "Chips Ahoy", "Batten Hatches", "Booty", "Crows Nest", "Jolly Roger", "Man-O-War", "Scallyway", "Calico Jack", "Captian Kidd", "Barbary Coast", "Cat O Nine", "Cutlass", "Cannon", "Powder Chest", "Kraken", "Parrot", "Wooden Leg", "Captian Hook", "Eye Patch", "Dread Pirate", "Hydra"};

int theme_len[9]={100,46,46,46,46,46,46,46,46};

char* level_list[10] ={*lab_general,*lab_long,*lab_homo,*lab_sym,*lab_emo,*lab_num,*lab_cons,*lab_adven,*lab_wars,*lab_pir};

void (*mods[9]) (char* str, uint8_t freq, uint8_t shift) ={Nada,CamelCase,Brnrd,Missing,Leet,Pig,Reverse,Scramble,Caesar};


void label_setup(_LEVEL *level){
	char string[17];
	char* stringp = string;
	uint8_t i=0;
	while(i<7){
		stringp=&(level_list[level->lab_theme])[level->lab_num[i]];
		(*mods[level->mod])(stringp,level->arg_freq, level->arg_shift);
		stringcpy(level->lab_str[i],stringp);
		i++;
	}
}

void label_print(_LEVEL *level){
	// Do printing things
}

void recieve_level(_LEVEL *level, uint8_t console){
	//Unpack into struct:
	// - label1
	// - lab_theme
	// - mod
	// - arg_freq
	// - arg_shift
	//FOR TESTING
	level->lab_num[0]=3;
	level->lab_num[1]=40;
	level->lab_num[2]=33;
	level->lab_num[3]=12;
	level->lab_num[4]=9;
	level->lab_num[5]=28;
	level->lab_theme=0;
	level->mod=0;
	level->arg_freq=0;
	level->arg_shift=0;
}

