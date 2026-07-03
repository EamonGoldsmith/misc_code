#include <string>
#include <iostream>

#define MESSAGE_TYPE_LIST \
	X(SetAlarLight,               "a") \
	X(SetWarningLight,            "b") \
	X(StatusString,               "c") \
	X(SetEjectBinPlacement,       "d") \
	X(HomeAll,                    "e") \
	X(DiagnosticsCommands,        "f") \
	X(DiagnosticsFunctions,       "g") \
	X(Subscription,               "h") \
	X(UnSubscribeDiagnostics,     "i") \
	X(DiagnosticsNamesList,       "j") \
	X(GetGaugeState,              "k") \
	X(SetGaugeState,              "l") \
	X(StartTest,                  "m") \
	X(StopTest,                   "n") \
	X(GaugeVersionInquiry,        "o") \
	X(Result,                     "p") \
	X(GetUIDescription,           "q") \

enum Type {
#define X(cmd, str) cmd,
	MESSAGE_TYPE_LIST
#undef X
};

std::string type_to_string(enum Type t)
{
	switch (t) {
#define X(cmd, str) case cmd: return #str; 
	MESSAGE_TYPE_LIST
#undef X
	default:
		return "zz";
	}
}

int main()
{
	std::cout << "test program\n";
	std::cout << type_to_string(StopTest) << std::endl;

	switch ("hello")
	{
		case "nothello":
			std::cout << "AAAH\n";
			break;
		case "hello":
			std::cout << "it works\n";
			break;
	}

	return 0;
}
