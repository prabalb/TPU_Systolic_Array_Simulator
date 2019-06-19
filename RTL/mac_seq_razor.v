module mac_seq(input [7:0] activ,
           input [7:0] weight,
           input clk,
           input del_clk,
           input [23:0] prev_mac_o,
           output [23:0] out,
           output reg error_out, // 0/1 signifies correct/erroneous computation
           output reg [7:0] prev_activ);

  wire [15:0] mult_out;
  wire [23:0] add_out;
  wire [23:0] del_add_out;

  assign out = add_out;

  mac_mult mac_mult_inst(
    .clk(clk),
    .operand_a(activ),
    .operand_b(weight),
    .out(mult_out)
  );

  mac_add mac_add_inst(
    .clk(clk),
    .del_clk(del_clk),
    .operand_a(prev_mac_o),
    .operand_b(mult_out),
    .out(add_out),
    .del_out(del_add_out)
  );

  always @ (posedge del_clk)
    if(del_add_out == add_out)
      error_out = 0;
    else
      error_out = 1;

  always @ (posedge clk)
    prev_activ <= activ; // store previous cycle's activation

endmodule

module mac_mult(
  input clk,
  input [7:0] operand_a,
  input [7:0] operand_b,
  output [15:0] out 
);
  reg[15:0] out_reg;
  assign out = out_reg;

  always @ (posedge clk)
    out_reg <= operand_a * operand_b;

endmodule


module mac_add(
  input clk,
  input del_clk,
  input [23:0] operand_a,
  input [15:0] operand_b,
  output [23:0] out,
  output [23:0] del_out
);

  reg[23:0] out_reg;
  reg[23:0] del_out_reg;
  assign out = out_reg;
  assign del_out = del_out_reg;
 
  always @ (posedge clk)
    out_reg <= operand_a + operand_b;

  always @ (posedge del_clk)
    del_out_reg <= operand_a + operand_b;

endmodule
