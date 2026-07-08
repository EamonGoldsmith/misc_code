module mux #(
  parameter int DATA_WIDTH = 8
)(
  input logic[DATA_WIDTH-1:0] a,
  input logic[DATA_WIDTH-1:0] b,
  input logic                 sel,
  output logic[DATA_WIDTH-1:0] o
);

  assign o = sel == 1 ? b : a;

endmodule : mux

module mux4 #(
  parameter int DATA_WIDTH = 8
) (
  input logic[DATA_WIDTH-1:0] a,
  input logic[DATA_WIDTH-1:0] b,
  input logic[DATA_WIDTH-1:0] c,
  input logic[DATA_WIDTH-1:0] d,
  input logic[           1:0] sel,
  output logic[DATA_WIDTH-1:0] o 
);

  logic [DATA_WIDTH-1:0] tmp1, tmp2;
  mux #(.DATA_WIDTH(DATA_WIDTH)) M1(.a(a), .b(b), .sel(sel[0]), .o(tmp1));
  mux #(.DATA_WIDTH(DATA_WIDTH)) M2(.a(c), .b(d), .sel(sel[0]), .o(tmp2));

  mux #(.DATA_WIDTH(DATA_WIDTH)) M3(.a(tmp1), .b(tmp2), .sel(sel[1]), .o(o));

endmodule : mux4
