library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity full_adder is
	port (i0, i1 : in std_logic_vector(3 downto 0);
	      ci : in std_logic;
	      s : out std_logic_vector(3 downto 0);
	      co : out std_logic);
end full_adder;

architecture rtl of full_adder is 
begin
	-- compute the sum
	s <= i0 xor i1 xor ci;

	-- compute the carry
	co <= (i0 and i1) or (i0 and ci) or (i1 and ci);
end rtl;
