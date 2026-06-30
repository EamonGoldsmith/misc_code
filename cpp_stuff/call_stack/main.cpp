#include <iostream>
#include <vector>

enum class Kind {
	Begin,
	End,
};

struct Event {
	Kind Type;
	double Elapsed;
	std::string Method;
};

using Events = std::vector<Event>;

struct Sample {
	double Elapsed;
	std::vector<std::string> Stack;
};

using Samples = std::vector<Sample>;

/*
	test input:
*/
Samples s = {
  { 2.4, { "main" } },
  { 2.5, { "main", "foo", "bar", } },
  { 2.6, { "main", "bar", "foo",  } },
  { 3.0, { "main", } },
};

bool stacks_match(std::vector<std::string> a, std::vector<std::string> b, int i)
{
	// can't match if nothing there
	if (a.size() - 1 < i || b.size() - 1 < i) {
		return false;
	}

	return a.at(i) == b.at(i);
}

int max(int a, int b)
{
	return a > b ? a : b;
}

bool stack_at(std::vector<std::string> stack, int i)
{
	if (stack.size() <= i) return false;
	else return true;
}

Events GenerateEvents(const Samples& samples)
{
	Events e;

	if (samples.empty()) {
		return e;
	}

	// first event
	Event f = { Kind::Begin, samples.at(0).Elapsed, samples.at(0).Stack.at(0) };
	e.push_back(f);

	// later events
	for (int j = 1; j < samples.size(); j++) {
		auto p = std::move(samples.at(j - 1).Stack);
		auto c = std::move(samples.at(j).Stack);

		// removed funcs
		for (int i = max(p.size(), c.size()) - 1; i >= 0; i--) {
			if (!stacks_match(p, c, i) && stack_at(p, i)) {
				Event n = { Kind::End, samples.at(j).Elapsed, p.at(i) };
				e.push_back(n);
			}
		}

		// added funcs
		for (int i = 0; i < max(p.size(), c.size()); i++) {
			if (!stacks_match(p, c, i) && stack_at(c, i)) {
				Event n = { Kind::Begin, samples.at(j).Elapsed, c.at(i) };
				e.push_back(n);
			}
		}
	}

	// last event
	f = { Kind::End, samples.back().Elapsed, samples.back().Stack.at(0) };
	e.push_back(f);

	return e;
}

int main()
{
	std::cout << "running tests\n";
	const auto e = GenerateEvents(s);

	std::cout << "results:\n";
	for (auto a : e) {
		std::cout << (a.Type == Kind::Begin ? "Begin" : "End") << ' ' << a.Elapsed << ' ' << a.Method << std::endl;
	}

	return 0;
}