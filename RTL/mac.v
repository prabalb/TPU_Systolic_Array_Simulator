module mac(input [7:0] activ,
           input [7:0] weight,
           input [23:0] prev_mac_o,
           output [23:0] out);

  wire [15:0] mult_out;
  wire [23:0] add_out;

  assign out = add_out;

  mac_mult mac_mult_inst(
    .operand_a(activ),
    .operand_b(weight),
    .out(mult_out)
  );

  mac_add mac_add_inst(
    .operand_a(prev_mac_o),
    .operand_b(mult_out),
    .out(add_out)
  );

endmodule

module mac_mult(
  input [7:0] operand_a,
  input [7:0] operand_b,
  output [15:0] out 
);
  assign out = operand_a * operand_b;

endmodule


module mac_add(
  input [23:0] operand_a,
  input [15:0] operand_b,
  output [23:0] out
);

  assign out = operand_a + operand_b;

endmodule
