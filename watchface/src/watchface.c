#include <pebble.h>

static Window *s_mWindow;
static TextLayer *s_tLayer;
static TextLayer *s_tLdaydate;
static GFont s_timeFont;
static BitmapLayer *s_backLayer;
static GBitmap *s_backBitMap;

static void updateTime() {
	time_t temp = time(NULL);
	struct tm *tckTime = localtime(&temp);
	
	static char s_buff[8];
	static char s_buffDD[20];
	strftime(s_buff, sizeof(s_buff), clock_is_24h_style() ? "%H:%M" : "%I:%M", tckTime);
	strftime(s_buffDD, sizeof(s_buffDD), "%A, %d'%b", tckTime);
	text_layer_set_text(s_tLayer, s_buff);
	text_layer_set_text(s_tLdaydate, s_buffDD);
}

static void tick_handler(struct tm *tckTime, TimeUnits unitChanged) {
	updateTime();
}

static void main_window_load(Window *window) {
	Layer *wLayer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(wLayer);
	
	s_backBitMap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	s_backLayer = bitmap_layer_create(bounds);
	s_timeFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MAKHINA_32));
	s_tLayer = text_layer_create(GRect(0,PBL_IF_ROUND_ELSE(78,72), bounds.size.w, 50));
	s_tLdaydate = text_layer_create(GRect(0,PBL_IF_ROUND_ELSE(147,145), bounds.size.w, 50));
	
	text_layer_set_background_color(s_tLayer, GColorClear);
	text_layer_set_text_color(s_tLayer, GColorBlack);
	// text_layer_set_text(s_tLayer, "00:00");
	text_layer_set_font(s_tLayer, s_timeFont); //fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD)
	text_layer_set_text_alignment(s_tLayer, GTextAlignmentCenter);
	
	text_layer_set_background_color(s_tLdaydate, GColorClear);
	text_layer_set_text_color(s_tLdaydate, GColorWhite);
	text_layer_set_font(s_tLdaydate, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(s_tLdaydate, GTextAlignmentCenter);
	
	bitmap_layer_set_bitmap(s_backLayer, s_backBitMap);
	layer_add_child(wLayer, bitmap_layer_get_layer(s_backLayer));
	layer_add_child(wLayer, text_layer_get_layer(s_tLayer));
	layer_add_child(wLayer, text_layer_get_layer(s_tLdaydate));
}

static void main_window_unload(Window *window) {
	text_layer_destroy(s_tLayer);
	text_layer_destroy(s_tLdaydate);
	fonts_unload_custom_font(s_timeFont);
	gbitmap_destroy(s_backBitMap);
	bitmap_layer_destroy(s_backLayer);
}

static void init() {
	s_mWindow = window_create();
	window_set_window_handlers(s_mWindow, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
		});
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	window_set_background_color(s_mWindow, GColorBlack);
	window_stack_push(s_mWindow, true);
}

static void deinit() {
	window_destroy(s_mWindow);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
