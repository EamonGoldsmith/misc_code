library ieee;
use ieee.std_logic1164.all;

entity mux_8to1 is
	port(
		DATA_in : in std_logic_vector(7 downto 0);
		SEL : in std_logic_vector(2 downto 0);
		CE : in std_logic;
		F_CTRL : out std_logic;
	);
end mux_8to1;

architecture mux_8to1_arc of mux_8to1 is
begin
	my_mux: process(DATA_in, SEL, CE)
	begin
		if (CE = '0') then
			F_CTRL <= '0';
		else
			if (SEL = "111") then F_CTRL <= Data_in(7);
			elsif (SEL = "110") then F_CTRL <= Data_in(6);
			elsif (SEL = "101") then F_CTRL <= Data_in(5);
			elsif (SEL = "100") then F_CTRL <= Data_in(4);
			elsif (SEL = "011") then F_CTRL <= Data_in(3);
			elsif (SEL = "010") then F_CTRL <= Data_in(2);
			elsif (SEL = "001") then F_CTRL <= Data_in(1);
			elsif (SEL = "000") then F_CTRL <= Data_in(0);
			else F_CTRL <= '0';
			end if;
		end if;
	end process my_mux;
end
