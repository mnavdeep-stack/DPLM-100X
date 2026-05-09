module modulator(
    input clk,
    input [2:0] mode,
    input digital_msg_in,
    input [7:0] sine_lut,
    input [11:0] input_signal,
    output reg [7:0] mod_out
);

reg signed [21:0] am_prod_reg;

always @(posedge clk) begin

    am_prod_reg <= ({1'b0, sine_lut} - 9'sd128) *
                   ({1'b0, input_signal} - 13'sd2048);

    case(mode)

        // AM
        3'd0:
            mod_out <= am_prod_reg[18:11] + 8'd128;

        // ASK
        3'd2:
            mod_out <= (digital_msg_in) ? sine_lut : 8'd128;

        // PSK
        3'd4:
            mod_out <= (digital_msg_in) ? sine_lut :
                        (255 - sine_lut);

        // FM / FSK default
        default:
            mod_out <= sine_lut;

    endcase
end

endmodule
