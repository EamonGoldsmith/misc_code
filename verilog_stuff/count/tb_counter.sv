`timescale 1ns/1ps

module tb_counter;

    // inputs
    logic clk;
    logic reset;

    // outputs
    logic [3:0] count;

    counter uut (
        .clk(clk),
        .reset(reset),
        .count(count)
    );

    // clock (toggles every 5 time units = 10ns period)
    always #5 clk = ~clk;

    initial begin
        $dumpfile("waveform.vcd");
        $dumpvars(0, tb_counter);

        // initialise inputs
        clk = 0;
        reset = 1;

        #20;
        reset = 0;

        #200;

        $display("Simulation finished. Open waveform.vcd in GTKWave.");
        $finish;
    end

endmodule
