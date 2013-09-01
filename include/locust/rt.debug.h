#define DEBUG_H

__attribute__((__format__ (__printf__, 6, 7)))
int __log(char const * const mode,
	      char const * const func_name,
	      char const * const file_name,
		  int line,
		  int do_die,
		  char const * const format,
		  ...);

#define die(...)   __log("DIE", __func__, __FILE__, __LINE__, 1, __VA_ARGS__)
#define error(...) __log("ERR", __func__, __FILE__, __LINE__, 0, __VA_ARGS__)
#define warn(...)  __log("WRN", __func__, __FILE__, __LINE__, 0, __VA_ARGS__)
#define info(...)  __log("INF", __func__, __FILE__, __LINE__, 0, __VA_ARGS__)
#define debug(...) __log("DBG", __func__, __FILE__, __LINE__, 0, __VA_ARGS__)

#define die_if(EXPR, ...)   ((EXPR) &&   die(__VA_ARGS__))
#define error_if(EXPR, ...) ((EXPR) && error(__VA_ARGS__))
#define warn_if(EXPR, ...)  ((EXPR) &&  warn(__VA_ARGS__))
#define info_if(EXPR, ...)  ((EXPR) &&  info(__VA_ARGS__))
#define debug_if(EXPR, ...) do { if (EXPR) { debug(__VA_ARGS__); }} while(0)

#define error_goto(...) do { error(__VA_ARGS__); goto error; } while(0)
#define warn_goto(...)  do {  warn(__VA_ARGS__); goto warn;  } while(0)
#define info_goto(...)  do {  info(__VA_ARGS__); goto info;  } while(0)
#define debug_goto(...) do { debug(__VA_ARGS__); goto debug; } while(0)

#define error_goto_if(EXPR, ...) do { if (EXPR) { error(__VA_ARGS__); goto error; }} while(0)
#define warn_goto_if(EXPR, ...)  do { if (EXPR) {  warn(__VA_ARGS__); goto warn;  }} while(0)
#define info_goto_if(EXPR, ...)  do { if (EXPR) {  info(__VA_ARGS__); goto info;  }} while(0)
#define debug_goto_if(EXPR, ...) do { if (EXPR) { debug(__VA_ARGS__); goto debug; }} while(0)

#define assert(EXPR) ((EXPR) ? (void)0 : die(#EXPR))


#ifdef NDEBUG

#define debug(...) ((void)0)
#define debug_if(EXPR, ...) ((void)0)
#define debug_goto_if(EXPR, ...) ((void)0)
#define debug_goto(...) ((void)0)
#define	assert(EXPR) ((void)0)

#endif
