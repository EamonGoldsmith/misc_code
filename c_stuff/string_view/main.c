#include <stdio.h>
#include <string.h>

#define ARENA_IMPLEMENTATION
#include "arena.h"

#define SV_IMPLEMENTATION
#include "sv.h"

String_View msg_chunk()
{
	static int i;

	switch (i++)
	{
	case 0: return SV("\x02" "h" "0000000000000000099999" "{vtr}");
	case 1: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 2: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 3: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 4: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 5: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 6: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 7: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 8: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 9: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 10: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 11: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 12: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 13: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 14: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 15: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 16: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 17: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 18: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 19: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 20: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 21: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 22: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 23: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 24: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 25: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 26: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 27: return SV("s" "0000000000000000099999" "{vtr:someother random crap}");
	case 28: return SV("h" "0000000000000000099999" "{vtr}" "\x03");
	}

	return SV("JIWOAJDIWOA");
}

// message predicates
bool is_not_message_start(char x)
{
	return !(x == '\x02' || x == '\x06' || x == '\x15');
}
bool is_not_message_end(char x)
{
	return (x != '\x03');
}
String_View trim_message(String_View *raw)
{
	String_View msg = sv_chop_left_while(raw, is_not_message_end);
	raw->count--; // remove message terminator from persistent

	if (msg.count == 0) {
		// message missing terminator, keep reading
		return SV_NULL;
	}

	// discard bytes before the start character
	(void) sv_chop_left_while(&msg, is_not_message_start);
	msg.count++; // include msg terminator

	// check start and end chars
	if (!(sv_starts_with(msg, SV("\x02"))
		|| sv_starts_with(msg, SV("\x06"))
		|| sv_starts_with(msg, SV("\x15")))
		|| !sv_ends_with(msg, SV("\x03")))
	{
		// trimmed message missing start or end
		return SV_NULL;
	}

	// check length
	if (msg.count < 24) {
		// received message is too short
		return SV_NULL;
	}

	return msg;
}

String_View get_msg(Arena *arena, String_Builder *sb)
{
	String_View chunk = msg_chunk();
	arena_sb_append_sv(arena, sb, chunk);

	// copy string builder so trim_message() doesn't modify it
	String_View builder; 
	builder.data = sb->items;
	builder.count = sb->count;

	String_View msg = trim_message(&builder);

	// failed to read complete message
	if (sv_eq(msg, SV_NULL)) {
		return SV_NULL;
	}

	// remove successfully read message from persistent
	(void) sv_chop_by_sv((String_View*)sb, msg);
	return msg;
}

int main()
{
	Arena scratchpad = {0};
	String_Builder sb = {0};

	String_View msg;

	for (int i = 0; i < 40; i++) {
		msg = get_msg(&scratchpad, &sb);

		if (sv_eq(msg, SV_NULL)) {
			printf("FAILED\n");
		} else {
			printf("YES READ: " SV_Fmt "\n", SV_Arg(msg));
		}
	}

	return 0;
}
