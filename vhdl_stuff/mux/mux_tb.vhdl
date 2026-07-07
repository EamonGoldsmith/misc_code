entity adder_tb is
end adder_tb;

architecture behav of adder_tb is
	component mux_8to1
		port (
			DATA_in : in std_logic_vector(7 downto 0);
			SEL : in std_logic_vector(2 downto 0);
			CE : in std_logic;
			F_CTRL : out std_logic;
		);
	end component;

	for mux_8to1_0: mux use entity work.mux_8to1

	signal DATA_in : in std_logic_vector(7 downto 0);
	signal SEL : in std_logic_vector(2 downto 0);
	signal CE : in std_logic;
	signal F_CTRL : out std_logic;
begin
	mux_8to1_0: adder port map (DATA_in => DATA_in, SEL => SEL, CE => CE, F_CTRL => F_CTRL);

	process

	type pattern_type is record
		Data_in : std_logic_vector(7 downto 0);
		Sel : std_logic_vector(2 downto 0);
		Ce, F_ctrl : std_logic;
	end record;

	type pattern_array is array (natural range <>) of pattern_type;
	constant patterns : pattern_array :=
	(('0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'),
	 ('0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'),
	 ('0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'),

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
