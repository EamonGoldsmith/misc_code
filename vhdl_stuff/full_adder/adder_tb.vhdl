library IEEE;
use IEEE.std_logic_1164.all;

-- a testbench has no ports
entity adder_tb is
end adder_tb;

architecture behav of adder_tb is 
	component adder
		port (i0, i1 : in std_logic; ci : in std_logic; s : out std_logic; co : out std_logic);
	end component;

	for adder_0: adder use entity work.adder;
	signal i0, i1, ci, s, co : std_logic;
begin

	adder_0: adder port map (i0 => i0 ,i1 => i1, ci => ci, s => s, co => co);

	process
		type pattern_type is record 
			i0, i1, ci : std_logic;
			s, co : std_logic;
		end record;

		type pattern_array is array (natural range <>) of pattern_type;
		constant patterns : pattern_array := (
		('0', '0', '0', '0', '0'),
		('0', '0', '1', '1', '0'),
		('0', '1', '0', '1', '0'),
		('0', '1', '1', '0', '1'),
		('1', '0', '0', '1', '0'),
		('1', '0', '1', '0', '1'),
		('1', '1', '0', '0', '1'),
		('1', '1', '1', '1', '1')
		);
	begin

		for i in patterns'range loop
			i0 <= patterns(i).i0;
			i1 <= patterns(i).i1;
			ci <= patterns(i).ci;

			wait for 1 ns;

			assert s = patterns(i).s 
				report "bad sum value" severity error;
			assert co = patterns(i).co
				report "bad carry out value" severity error;
		end loop;

		assert false report "end of test" severity note;
		wait;

	end process;
end behav;
