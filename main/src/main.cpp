#include <iostream>
#include <string>
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include "structure.h"
#include "m2_helper.h"
#include <ctime>
#include <time.h>
#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)


extern streetGraph street_graph;

int main(int argc, char** argv) {

    std::string map_path;
    if (argc == 1) {
        //Use a default map
        map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
    } else if (argc == 2) {
        //Get the map from the command line
        map_path = argv[1];
    } else {
        //Invalid arguments
        std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
        std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
        return 1;
    }

    //Load the map and related data structures
    bool load_success = load_map(map_path);
    if (!load_success) {
        std::cerr << "Failed to load map '" << map_path << "'\n";
        return 2;
    }

    std::cout << "Successfully loaded map '" << map_path << "'\n";

    /*std::vector<DeliveryInfo> deliveries = {DeliveryInfo(70831, 51733), DeliveryInfo(64614, 49104), DeliveryInfo(86395, 90198), DeliveryInfo(68915, 1748), DeliveryInfo(77817, 36464), DeliveryInfo(7906, 86992), DeliveryInfo(85195, 97964), DeliveryInfo(24366, 11113), DeliveryInfo(62657, 72429), DeliveryInfo(44826, 33045), DeliveryInfo(6566, 86491), DeliveryInfo(91850, 60035), DeliveryInfo(36280, 57275), DeliveryInfo(97481, 1195), DeliveryInfo(68300, 60860), DeliveryInfo(77791, 15128), DeliveryInfo(69739, 8078), DeliveryInfo(68385, 108770), DeliveryInfo(75459, 100013), DeliveryInfo(67285, 100826), DeliveryInfo(16103, 47349), DeliveryInfo(44690, 30333), DeliveryInfo(989, 81045), DeliveryInfo(95265, 10843), DeliveryInfo(15804, 90041), DeliveryInfo(66262, 30923), DeliveryInfo(69352, 11124), DeliveryInfo(39741, 80709), DeliveryInfo(107067, 9155), DeliveryInfo(9727, 87561), DeliveryInfo(14679, 69801), DeliveryInfo(49350, 45035), DeliveryInfo(46806, 39996), DeliveryInfo(22340, 50841), DeliveryInfo(83735, 29110), DeliveryInfo(84125, 35208), DeliveryInfo(27827, 94577), DeliveryInfo(99880, 45391), DeliveryInfo(51371, 56818), DeliveryInfo(108031, 19488), DeliveryInfo(41567, 48318), DeliveryInfo(2266, 5702), DeliveryInfo(82273, 58171), DeliveryInfo(105662, 92219), DeliveryInfo(71511, 93073), DeliveryInfo(76398, 49443), DeliveryInfo(44689, 105495), DeliveryInfo(20806, 49666), DeliveryInfo(74292, 43619), DeliveryInfo(99932, 46063), DeliveryInfo(67216, 82420), DeliveryInfo(83186, 45317), DeliveryInfo(76221, 39119), DeliveryInfo(23822, 101787), DeliveryInfo(30252, 47210), DeliveryInfo(21261, 30777), DeliveryInfo(32640, 60454), DeliveryInfo(5132, 5321), DeliveryInfo(36493, 6120), DeliveryInfo(57286, 100769), DeliveryInfo(96376, 26250), DeliveryInfo(51728, 103554), DeliveryInfo(40558, 39484), DeliveryInfo(2725, 55441), DeliveryInfo(74490, 26052), DeliveryInfo(39219, 29203), DeliveryInfo(81784, 3342), DeliveryInfo(9007, 54902), DeliveryInfo(74175, 27755), DeliveryInfo(21235, 22648), DeliveryInfo(53109, 15924), DeliveryInfo(4863, 44723), DeliveryInfo(37191, 76062), DeliveryInfo(88341, 85383), DeliveryInfo(94683, 55071), DeliveryInfo(36870, 22371), DeliveryInfo(27010, 53986), DeliveryInfo(4562, 6641), DeliveryInfo(54939, 83587), DeliveryInfo(56452, 69532), DeliveryInfo(98765, 72206), DeliveryInfo(57705, 18710), DeliveryInfo(93630, 76760), DeliveryInfo(75607, 23835), DeliveryInfo(50531, 62871), DeliveryInfo(87576, 103929), DeliveryInfo(75119, 104926), DeliveryInfo(28917, 94863), DeliveryInfo(48014, 79421), DeliveryInfo(67807, 70881), DeliveryInfo(36713, 40596), DeliveryInfo(60356, 107976), DeliveryInfo(61155, 49594), DeliveryInfo(5404, 68375), DeliveryInfo(86324, 87156), DeliveryInfo(58034, 43156), DeliveryInfo(26541, 93050), DeliveryInfo(49782, 56269), DeliveryInfo(98805, 88285), DeliveryInfo(96318, 57406), DeliveryInfo(3909, 1063), DeliveryInfo(106669, 38602), DeliveryInfo(60559, 39417), DeliveryInfo(32310, 77756), DeliveryInfo(43215, 79559), DeliveryInfo(2719, 85706), DeliveryInfo(44546, 50836), DeliveryInfo(103884, 51061), DeliveryInfo(33352, 97894), DeliveryInfo(13982, 53927), DeliveryInfo(39136, 83642), DeliveryInfo(16454, 16407), DeliveryInfo(107566, 10186), DeliveryInfo(55738, 29559), DeliveryInfo(20433, 64640), DeliveryInfo(51018, 47704), DeliveryInfo(83418, 20963), DeliveryInfo(46690, 88588), DeliveryInfo(79849, 202), DeliveryInfo(74264, 55481), DeliveryInfo(86580, 46058), DeliveryInfo(41171, 66137), DeliveryInfo(75263, 31994), DeliveryInfo(9586, 45923), DeliveryInfo(103834, 69443), DeliveryInfo(50331, 53154), DeliveryInfo(60773, 99528), DeliveryInfo(55406, 19758), DeliveryInfo(6145, 89729), DeliveryInfo(108448, 58740), DeliveryInfo(81702, 13838), DeliveryInfo(103362, 31397), DeliveryInfo(54844, 92286), DeliveryInfo(43567, 69760), DeliveryInfo(79978, 86281), DeliveryInfo(22727, 51589), DeliveryInfo(80415, 31898), DeliveryInfo(94398, 36759), DeliveryInfo(85643, 11886), DeliveryInfo(99530, 91160), DeliveryInfo(24362, 91302), DeliveryInfo(61012, 103837), DeliveryInfo(65021, 32757), DeliveryInfo(51692, 101825), DeliveryInfo(98287, 68477), DeliveryInfo(61805, 90143), DeliveryInfo(92611, 90642), DeliveryInfo(89383, 26267), DeliveryInfo(107584, 16088), DeliveryInfo(26525, 77272), DeliveryInfo(82828, 107683), DeliveryInfo(47068, 107059), DeliveryInfo(41357, 87541), DeliveryInfo(1224, 69355), DeliveryInfo(25809, 18541), DeliveryInfo(98564, 427), DeliveryInfo(68616, 101140), DeliveryInfo(41462, 75845), DeliveryInfo(84210, 94998), DeliveryInfo(47770, 5933), DeliveryInfo(74323, 20719), DeliveryInfo(96722, 88605), DeliveryInfo(98608, 101900), DeliveryInfo(16936, 27083), DeliveryInfo(47448, 39881), DeliveryInfo(74333, 43454), DeliveryInfo(18735, 66274), DeliveryInfo(51610, 59328), DeliveryInfo(7572, 44358), DeliveryInfo(96967, 76061), DeliveryInfo(20466, 30814), DeliveryInfo(78564, 5062), DeliveryInfo(102853, 7788), DeliveryInfo(47378, 8940), DeliveryInfo(4738, 45674)};
    std::vector<unsigned> depots = {14, 55539, 66199, 38064, 87930, 15037, 9781, 42243, 62859, 50192, 58489, 39444, 53709, 93336, 60037, 7650, 28282, 105649, 38892, 87051};
    float turn_penalty = 15;

    std::vector<unsigned> result_path = traveling_courier(deliveries, depots, turn_penalty);
    
    std::cout << "Test finished " << std::endl;*/
    
    /*std::vector<DeliveryInfo> deliveries = {DeliveryInfo(102343, 40664), DeliveryInfo(66501, 33150), DeliveryInfo(64220, 70192), DeliveryInfo(10143, 89556), DeliveryInfo(51798, 85214)};
    std::vector<unsigned> depots = {14526, 21436, 41416};
    float turn_penalty = 15;
    std::vector<unsigned> result_path = traveling_courier(deliveries, depots, turn_penalty);

    std::cout << "Test finished " << std::endl;*/

    /*std::vector<DeliveryInfo> deliveries = {DeliveryInfo(73593, 104453), DeliveryInfo(1981, 47060), DeliveryInfo(58204, 31403), DeliveryInfo(102129, 63975), DeliveryInfo(41153, 63608)};
    std::vector<unsigned> depots = {2521, 5153, 41138};
    float turn_penalty = 15;
    std::vector<unsigned> result_path = traveling_courier(deliveries, depots, turn_penalty);*/

    /*std::vector<DeliveryInfo> deliveries = {DeliveryInfo(102168, 46017), DeliveryInfo(59033, 3949), DeliveryInfo(52607, 70747), DeliveryInfo(62536, 100049), DeliveryInfo(87300, 100878), DeliveryInfo(61813, 23189), DeliveryInfo(103299, 82337), DeliveryInfo(87922, 6329), DeliveryInfo(106883, 61250), DeliveryInfo(74157, 81025), DeliveryInfo(107776, 82337), DeliveryInfo(24454, 83613), DeliveryInfo(89223, 26925), DeliveryInfo(22739, 82458), DeliveryInfo(96372, 26433), DeliveryInfo(56696, 21665), DeliveryInfo(94993, 64798), DeliveryInfo(103248, 35208), DeliveryInfo(68697, 105256), DeliveryInfo(105074, 73527), DeliveryInfo(14179, 3364), DeliveryInfo(20302, 23189), DeliveryInfo(97506, 81025), DeliveryInfo(51178, 62599), DeliveryInfo(94080, 80313), DeliveryInfo(39037, 63926), DeliveryInfo(45703, 90270), DeliveryInfo(108282, 64030), DeliveryInfo(7971, 96808), DeliveryInfo(53625, 82969), DeliveryInfo(62121, 7506), DeliveryInfo(38448, 83330), DeliveryInfo(89653, 73370), DeliveryInfo(70104, 23189), DeliveryInfo(103248, 22508), DeliveryInfo(36693, 53342), DeliveryInfo(106372, 81025), DeliveryInfo(103248, 56701), DeliveryInfo(48740, 82337), DeliveryInfo(48519, 22647), DeliveryInfo(106184, 12268), DeliveryInfo(107776, 105310), DeliveryInfo(107776, 87790), DeliveryInfo(50962, 1044), DeliveryInfo(103248, 59901), DeliveryInfo(41485, 2487), DeliveryInfo(54156, 35650), DeliveryInfo(103854, 788), DeliveryInfo(90686, 19604), DeliveryInfo(12849, 23966), DeliveryInfo(104727, 82337), DeliveryInfo(34239, 87043), DeliveryInfo(97438, 43353), DeliveryInfo(103887, 19163), DeliveryInfo(39005, 56987), DeliveryInfo(17558, 94519), DeliveryInfo(21142, 60659), DeliveryInfo(90026, 92605), DeliveryInfo(52852, 73529), DeliveryInfo(5207, 12090), DeliveryInfo(26171, 5120), DeliveryInfo(17800, 7514), DeliveryInfo(60805, 82874), DeliveryInfo(41711, 81630), DeliveryInfo(104428, 104847), DeliveryInfo(107744, 25649), DeliveryInfo(12111, 50400), DeliveryInfo(38362, 59881), DeliveryInfo(50843, 19784), DeliveryInfo(9088, 23189), DeliveryInfo(39544, 72147), DeliveryInfo(107776, 1708), DeliveryInfo(108614, 93653), DeliveryInfo(82136, 13562), DeliveryInfo(104671, 52031), DeliveryInfo(50283, 97305), DeliveryInfo(103298, 30594), DeliveryInfo(73397, 93338), DeliveryInfo(29944, 98197), DeliveryInfo(31562, 89513), DeliveryInfo(7644, 98949), DeliveryInfo(86296, 33118), DeliveryInfo(99187, 83233), DeliveryInfo(30362, 105801), DeliveryInfo(59268, 27793), DeliveryInfo(78113, 88982), DeliveryInfo(36306, 99377), DeliveryInfo(98705, 81149), DeliveryInfo(5162, 34639), DeliveryInfo(55265, 37355), DeliveryInfo(49864, 93912), DeliveryInfo(78527, 88186), DeliveryInfo(75827, 17720), DeliveryInfo(40339, 21829), DeliveryInfo(98173, 58391), DeliveryInfo(103248, 99952), DeliveryInfo(58155, 18617), DeliveryInfo(52214, 66081), DeliveryInfo(28847, 15478), DeliveryInfo(104461, 56342)};
    std::vector<unsigned> depots = {12, 46283, 736};
    float turn_penalty = 15;
    std::vector<unsigned> result_path = traveling_courier(deliveries, depots, turn_penalty);

    std::cout << "Test finished " << std::endl;*/

    /*std::vector<DeliveryInfo> deliveries = {DeliveryInfo(79455, 73885), DeliveryInfo(14430, 10485), DeliveryInfo(79455, 99841), DeliveryInfo(108559, 26485), DeliveryInfo(38749, 65506), DeliveryInfo(20835, 87067), DeliveryInfo(84376, 107360), DeliveryInfo(42725, 29784), DeliveryInfo(32762, 75186), DeliveryInfo(34213, 39393), DeliveryInfo(69426, 107360), DeliveryInfo(55325, 28231), DeliveryInfo(33647, 31763), DeliveryInfo(68294, 15356), DeliveryInfo(70155, 24347), DeliveryInfo(28514, 55990), DeliveryInfo(46387, 88018), DeliveryInfo(37630, 15626), DeliveryInfo(86263, 58035), DeliveryInfo(82461, 20408), DeliveryInfo(91578, 24347), DeliveryInfo(107158, 42714), DeliveryInfo(35313, 7843), DeliveryInfo(17403, 47053), DeliveryInfo(77278, 63670), DeliveryInfo(37630, 93854), DeliveryInfo(33647, 107676), DeliveryInfo(37995, 30867), DeliveryInfo(71720, 73392), DeliveryInfo(4875, 89987), DeliveryInfo(57530, 37233), DeliveryInfo(71726, 50174), DeliveryInfo(47045, 15356), DeliveryInfo(97081, 15356), DeliveryInfo(34213, 67172), DeliveryInfo(80829, 63670), DeliveryInfo(103400, 101884), DeliveryInfo(107667, 64172), DeliveryInfo(57530, 83862), DeliveryInfo(26535, 101729), DeliveryInfo(578, 4701), DeliveryInfo(82, 2027), DeliveryInfo(20059, 51404), DeliveryInfo(43367, 54326), DeliveryInfo(37995, 97563), DeliveryInfo(6469, 102816), DeliveryInfo(106029, 42732), DeliveryInfo(36836, 106282), DeliveryInfo(71616, 12665), DeliveryInfo(17189, 31763), DeliveryInfo(51160, 63670), DeliveryInfo(2030, 1781), DeliveryInfo(52135, 58035), DeliveryInfo(60829, 38153), DeliveryInfo(70329, 27322), DeliveryInfo(56687, 50174), DeliveryInfo(102644, 93854), DeliveryInfo(51160, 21629), DeliveryInfo(89837, 21123), DeliveryInfo(62814, 44061), DeliveryInfo(5485, 4477), DeliveryInfo(33647, 65506), DeliveryInfo(51160, 75466), DeliveryInfo(67195, 14706), DeliveryInfo(51160, 7843), DeliveryInfo(37995, 12665), DeliveryInfo(107053, 86249), DeliveryInfo(79455, 70851), DeliveryInfo(20490, 59387), DeliveryInfo(51160, 77914), DeliveryInfo(79455, 80541), DeliveryInfo(103954, 30634), DeliveryInfo(100065, 20408), DeliveryInfo(83317, 38615), DeliveryInfo(45974, 20321), DeliveryInfo(35313, 1899), DeliveryInfo(62038, 67929), DeliveryInfo(70710, 57250), DeliveryInfo(5225, 53901), DeliveryInfo(82993, 33449), DeliveryInfo(51160, 49192), DeliveryInfo(69411, 43920), DeliveryInfo(57530, 48581), DeliveryInfo(34213, 12965), DeliveryInfo(99394, 17430), DeliveryInfo(12367, 10485), DeliveryInfo(82993, 80188), DeliveryInfo(56284, 56652), DeliveryInfo(37630, 93854), DeliveryInfo(34213, 9757), DeliveryInfo(8831, 81670), DeliveryInfo(67383, 70107), DeliveryInfo(66198, 105775), DeliveryInfo(72195, 20160), DeliveryInfo(51160, 4165), DeliveryInfo(52135, 84657), DeliveryInfo(51724, 59909), DeliveryInfo(34213, 33857), DeliveryInfo(22254, 96331), DeliveryInfo(2996, 24081), DeliveryInfo(19118, 53115), DeliveryInfo(79455, 30052), DeliveryInfo(40790, 107605), DeliveryInfo(79455, 63670), DeliveryInfo(62255, 36331), DeliveryInfo(99394, 63670), DeliveryInfo(61183, 67293), DeliveryInfo(107158, 65506), DeliveryInfo(92100, 30634), DeliveryInfo(61789, 5972), DeliveryInfo(87640, 7843), DeliveryInfo(69768, 80701), DeliveryInfo(105434, 67172), DeliveryInfo(44077, 12413), DeliveryInfo(37630, 25767), DeliveryInfo(51160, 50383), DeliveryInfo(32528, 103466), DeliveryInfo(37995, 4701), DeliveryInfo(37995, 73392), DeliveryInfo(105174, 12665), DeliveryInfo(66884, 68455), DeliveryInfo(97034, 38752), DeliveryInfo(107158, 84254), DeliveryInfo(38421, 54525), DeliveryInfo(26272, 38594), DeliveryInfo(46996, 12665), DeliveryInfo(35313, 76583), DeliveryInfo(39295, 7843), DeliveryInfo(24117, 10485), DeliveryInfo(66930, 57489), DeliveryInfo(27823, 4165), DeliveryInfo(55810, 107360), DeliveryInfo(35313, 80701), DeliveryInfo(37995, 73198), DeliveryInfo(29102, 43920), DeliveryInfo(6709, 52971), DeliveryInfo(39078, 69259), DeliveryInfo(99149, 7843), DeliveryInfo(102632, 15005), DeliveryInfo(105744, 30061), DeliveryInfo(87027, 50174), DeliveryInfo(40801, 69027), DeliveryInfo(6709, 80541), DeliveryInfo(107158, 97563), DeliveryInfo(26214, 31702), DeliveryInfo(77961, 20408), DeliveryInfo(35313, 74120), DeliveryInfo(1181, 59203), DeliveryInfo(83990, 33468), DeliveryInfo(26750, 15356), DeliveryInfo(37310, 20307), DeliveryInfo(79455, 73392), DeliveryInfo(102613, 3112), DeliveryInfo(60258, 67985), DeliveryInfo(10679, 27776), DeliveryInfo(40080, 58035), DeliveryInfo(99394, 26570), DeliveryInfo(57530, 31568), DeliveryInfo(3153, 22846), DeliveryInfo(42725, 93247), DeliveryInfo(49298, 92319), DeliveryInfo(6345, 13222), DeliveryInfo(59248, 38615), DeliveryInfo(91580, 21629), DeliveryInfo(2032, 98262), DeliveryInfo(80474, 38685), DeliveryInfo(33647, 68676), DeliveryInfo(35313, 60835), DeliveryInfo(83317, 28756), DeliveryInfo(99394, 59203), DeliveryInfo(86012, 93854), DeliveryInfo(47947, 21629), DeliveryInfo(14774, 65506), DeliveryInfo(79455, 58035), DeliveryInfo(23741, 46990), DeliveryInfo(102613, 10998), DeliveryInfo(108063, 15726), DeliveryInfo(41604, 6224), DeliveryInfo(104009, 73603), DeliveryInfo(75284, 67344), DeliveryInfo(6709, 104642), DeliveryInfo(101640, 7182), DeliveryInfo(36071, 54387), DeliveryInfo(98893, 33156), DeliveryInfo(30885, 107331), DeliveryInfo(82993, 66932), DeliveryInfo(33647, 31763), DeliveryInfo(24490, 92319), DeliveryInfo(57530, 57345), DeliveryInfo(12689, 59352), DeliveryInfo(6709, 65506), DeliveryInfo(103954, 50150), DeliveryInfo(1959, 16171), DeliveryInfo(20490, 98262), DeliveryInfo(8166, 56081), DeliveryInfo(34213, 71797), DeliveryInfo(31305, 67172), DeliveryInfo(24244, 21629), DeliveryInfo(75089, 97563), DeliveryInfo(7169, 97563), DeliveryInfo(30301, 67539), DeliveryInfo(90791, 81955), DeliveryInfo(42959, 59203), DeliveryInfo(57530, 61808), DeliveryInfo(410, 92494), DeliveryInfo(92349, 40334), DeliveryInfo(91580, 18045), DeliveryInfo(64993, 82804), DeliveryInfo(65560, 15298), DeliveryInfo(107158, 48468), DeliveryInfo(79455, 20408), DeliveryInfo(42725, 8731), DeliveryInfo(42725, 81558), DeliveryInfo(27119, 99648), DeliveryInfo(37630, 59203), DeliveryInfo(51160, 65981), DeliveryInfo(102613, 32994), DeliveryInfo(34797, 12045), DeliveryInfo(2237, 80710), DeliveryInfo(91580, 5061), DeliveryInfo(34213, 91631), DeliveryInfo(105744, 80541), DeliveryInfo(6226, 38594), DeliveryInfo(7394, 73392), DeliveryInfo(102613, 38594), DeliveryInfo(82871, 15356), DeliveryInfo(102887, 20408), DeliveryInfo(105744, 24347), DeliveryInfo(63589, 7550), DeliveryInfo(102613, 59324), DeliveryInfo(99394, 8553), DeliveryInfo(103589, 21617), DeliveryInfo(51160, 59203), DeliveryInfo(51813, 60651), DeliveryInfo(90255, 351), DeliveryInfo(37630, 37233), DeliveryInfo(102613, 74991), DeliveryInfo(71605, 38594), DeliveryInfo(37995, 10985), DeliveryInfo(99394, 7971), DeliveryInfo(12087, 73392), DeliveryInfo(5446, 11574), DeliveryInfo(52117, 107386), DeliveryInfo(35313, 21303), DeliveryInfo(71021, 79876), DeliveryInfo(57530, 67172), DeliveryInfo(79455, 12665), DeliveryInfo(50874, 70919), DeliveryInfo(102613, 31763), DeliveryInfo(34014, 66982), DeliveryInfo(6372, 98642), DeliveryInfo(1958, 10485), DeliveryInfo(52135, 29121), DeliveryInfo(52135, 31763), DeliveryInfo(54404, 12767), DeliveryInfo(34213, 6543), DeliveryInfo(3238, 13116), DeliveryInfo(99394, 89087), DeliveryInfo(102613, 43626), DeliveryInfo(97803, 59091)};
    std::vector<unsigned> depots = {80658, 107155, 34524, 106290, 80008};
    float turn_penalty = 15;

    std::vector<unsigned> result_path = traveling_courier(deliveries, depots, turn_penalty);

    std::cout << "Test finished " << std::endl;*/

//    std::vector<DeliveryInfo> deliveries = {DeliveryInfo(102343, 40664), DeliveryInfo(66501, 33150), DeliveryInfo(64220, 70192), DeliveryInfo(10143, 89556), DeliveryInfo(51798, 85214)};
//    std::vector<unsigned> depots = {14526, 21436, 41416};
//    float turn_penalty = 15;
//    std::vector<unsigned> result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(9256, 65463)};
//    depots = {2};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(9140, 19741), DeliveryInfo(67133, 63045)};
//    depots = {24341, 84950};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(7552, 11395), DeliveryInfo(20877, 76067), DeliveryInfo(76915, 76067), DeliveryInfo(108204, 76067), DeliveryInfo(108204, 99189), DeliveryInfo(32523, 99189), DeliveryInfo(32523, 92242), DeliveryInfo(32523, 99189), DeliveryInfo(32523, 52119)};
//    depots = {32736, 27838, 28149};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(82141, 93835), DeliveryInfo(72558, 3972), DeliveryInfo(56282, 90416), DeliveryInfo(107044, 44237), DeliveryInfo(67389, 44237), DeliveryInfo(80027, 98370), DeliveryInfo(99714, 93835), DeliveryInfo(20449, 93835)};
//    depots = {33451, 24583, 88889};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(43046, 85976)};
//    depots = {33578, 56051};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(51656, 73523), DeliveryInfo(13931, 48963), DeliveryInfo(59889, 44527), DeliveryInfo(43679, 67308), DeliveryInfo(22657, 81046)};
//    depots = {36090, 38968, 33098};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(42364, 28031), DeliveryInfo(47309, 59485), DeliveryInfo(19950, 83619), DeliveryInfo(19950, 28031), DeliveryInfo(15381, 59485), DeliveryInfo(42364, 59485), DeliveryInfo(19950, 66464), DeliveryInfo(89161, 21391), DeliveryInfo(19950, 28031)};
//    depots = {42952, 27383, 50571};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(18320, 85366), DeliveryInfo(59662, 97286), DeliveryInfo(98769, 79154), DeliveryInfo(81171, 35000), DeliveryInfo(89377, 15810)};
//    depots = {53592, 35108, 73562};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(43327, 28619)};
//    depots = {56060};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(103810, 3723), DeliveryInfo(34578, 90920), DeliveryInfo(76994, 35921), DeliveryInfo(47909, 90920), DeliveryInfo(80021, 39409), DeliveryInfo(106449, 35921), DeliveryInfo(22113, 35921), DeliveryInfo(5385, 95758)};
//    depots = {58218, 19461, 56760};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(52747, 24225), DeliveryInfo(52747, 22444), DeliveryInfo(12519, 65813), DeliveryInfo(52747, 62211), DeliveryInfo(26565, 72357), DeliveryInfo(18521, 75984), DeliveryInfo(80021, 789), DeliveryInfo(80021, 50674)};
//    depots = {58853, 4637, 59870};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(64489, 10192), DeliveryInfo(66348, 47055)};
//    depots = {75020, 59249};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//
//    deliveries = {DeliveryInfo(7261, 90648), DeliveryInfo(42431, 20777), DeliveryInfo(18005, 104347)};
//    depots = {63031, 73383, 10168};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(55705, 95430), DeliveryInfo(108325, 79055)};
//    depots = {64436};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(53600, 84132), DeliveryInfo(7801, 20885)};
//    depots = {65267, 91978};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(14020, 18998), DeliveryInfo(48091, 4060), DeliveryInfo(54883, 50882)};
//    depots = {36451, 49616, 8297};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(23069, 80185), DeliveryInfo(23459, 60949), DeliveryInfo(62940, 18214), DeliveryInfo(81951, 19441), DeliveryInfo(86025, 46362)};
//    depots = {80138, 25819, 5507};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(9748, 61004), DeliveryInfo(63381, 88129)};
//    depots = {80938};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(90183, 84955), DeliveryInfo(57973, 86285), DeliveryInfo(57973, 63719), DeliveryInfo(99338, 85306), DeliveryInfo(1024, 16646), DeliveryInfo(86476, 62274), DeliveryInfo(57973, 103682), DeliveryInfo(90183, 16062)};
//    depots = {81982, 75396, 34731};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(44848, 98941), DeliveryInfo(108022, 62818), DeliveryInfo(17844, 76041)};
//    depots = {80481, 34196, 47772};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(15508, 102530), DeliveryInfo(91823, 22518), DeliveryInfo(104224, 42933), DeliveryInfo(91823, 102530), DeliveryInfo(79815, 102530), DeliveryInfo(76026, 19841), DeliveryInfo(76026, 22518), DeliveryInfo(76026, 19489), DeliveryInfo(76026, 22518)};
//    depots = {27131, 69454, 50227};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//    
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(105371, 20649)};
//    depots = {97060};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(17930, 93400)};
//    depots = {98141, 104672};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(101919, 45138)};
//    depots = {98719, 32007};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(25692, 87297), DeliveryInfo(33040, 6927), DeliveryInfo(104990, 106963), DeliveryInfo(99873, 6118), DeliveryInfo(24190, 106963), DeliveryInfo(66817, 6927), DeliveryInfo(48830, 6927), DeliveryInfo(47500, 107097)};
//    depots = {99219, 33748, 86378};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(26409, 44178), DeliveryInfo(71224, 80079), DeliveryInfo(102350, 45533), DeliveryInfo(64908, 100186), DeliveryInfo(98394, 12677)};
//    depots = {104947, 105823, 71751};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(32342, 46379), DeliveryInfo(97919, 71085)};
//    depots = {105225};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//    
//    deliveries = {DeliveryInfo(29579, 21548), DeliveryInfo(73225, 16427), DeliveryInfo(73225, 27760), DeliveryInfo(67738, 66845), DeliveryInfo(67738, 63977), DeliveryInfo(73225, 86972), DeliveryInfo(64559, 97609), DeliveryInfo(29444, 43148)};
//    depots = {107859, 12362, 94777};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    //std::cout << "Test finished " << std::endl;
//
//    /*deliveries = {DeliveryInfo(102343, 40664), DeliveryInfo(66501, 33150), DeliveryInfo(64220, 70192), DeliveryInfo(10143, 89556), DeliveryInfo(51798, 85214)};
//    depots = {14526, 21436, 41416};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);
//
//    deliveries = {DeliveryInfo(9256, 65463)};
//    depots = {2};
//    turn_penalty = 15;
//    result_path = traveling_courier(deliveries, depots, turn_penalty);*/
//
//    /*std::vector<DeliveryInfo> 
//     * 
//     * std::vector<unsigned>*/
//
//
//    /*for (unsigned i = 0; i < result_path.size(); i++) {
//
//        StreetSegmentInfo info = getStreetSegmentInfo(result_path[i]);
//
//        std::cout << "----->  " << info.from << std::endl;
//
//        std::cout << "----->  " << info.to << "\n\n" << std::endl;
//    }*/

    draw_map();
    std::cout << "Successfully draw map '" << map_path << "'\n";

    //Clean-up the map related data structures
    std::cout << "Closing map\n";
    close_map(); 
    
    
    return 0;
}
