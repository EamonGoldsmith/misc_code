//usr/bin/cc -o ${o=`mktemp`} "$0" -trigraphs --no-warnings && exec -a "$0" "$o" "$@"/

%:include <stdio.h>

%:define BUFSIZE  512
int arr<:BUFSIZE:> = <%1, 3, 2, 5%>;

typedef struct <%
	char* msg;
	int cnt;
%> params;

_print_msg(params p) <%
	for (int i = p.cnt<:arr:>; i > 0; i--) <%
		printf("Hello, %s??/n", p.msg);
	%>
%>

#define print_msg(...) ??/
	_print_msg( (params) <% .msg = "Nothing", .cnt = 1, __VA_ARGS__ %> );

int main()
<%
	print_msg(.msg = "World!", .cnt = 3);
	print_msg(.cnt = 0);

	return 0;
%>
