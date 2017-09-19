#include "m1.cpp"
#include "m2.h"
#include "m2_helper.h"
#include "StreetsDatabaseAPI.h"
#include "structure.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <math.h>
#include "Surface.h"
#include "easygl/easygl_constants.h"
#include "fontcache.h"
#include "graphics.h"
#include "graphics_automark.h"
#include "graphics_state.h"
#include "graphics_types.h"

extern std::vector<double> boundary;

double boundary_area;

void draw_map() {

    t_bound_box initial_cords = t_bound_box(boundary[3], boundary[2], boundary[1], boundary[0]);

    std::string mapName = "The Great Peggie Map";

    t_color backGround(220, 220, 220);

    init_graphics(mapName, backGround);

    set_drawing_buffer(ON_SCREEN);

    set_visible_world(initial_cords);

    boundary_area = get_visible_world().area();

    update_message("Curren City: Totonto Canada");

    destroy_button("Window");

    create_button("Zoom Fit", "City", act_on_city);
    create_button("City", "Reset", act_on_reset);
    create_button("Reset", "Find", act_on_search_intersection);
    create_button("Find", "Street", act_on_search_Street);
    create_button("Street", "Path1", act_on_search_path_intersection);
    create_button("Path1", "Path2", act_on_search_path_POI);
    create_button("Path2", "Intersection", act_on_find_intersection);
    create_button("Intersection", "POI", act_on_find_POI);
    create_button("POI", "Transportation", act_on_find_transportation);
    create_button("Transportation", "Help", act_on_help);

    set_keypress_input(true);

    set_mouse_move_input(true);

    event_loop(act_on_button_press, NULL, NULL, drawscreen);

    t_bound_box old_coords = get_visible_world();



}
