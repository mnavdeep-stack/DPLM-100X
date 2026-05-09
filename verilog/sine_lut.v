module sine_lut_generator(
    input [7:0] carrier_phase,
    output reg [7:0] sine_lut
);

always @(*) begin
    case(carrier_phase[7:3])
        5'd0:  sine_lut = 128;
        5'd1:  sine_lut = 153;
        5'd2:  sine_lut = 177;
        5'd3:  sine_lut = 199;
        5'd4:  sine_lut = 219;
        5'd5:  sine_lut = 234;
        5'd6:  sine_lut = 246;
        5'd7:  sine_lut = 253;
        5'd8:  sine_lut = 255;
        5'd9:  sine_lut = 253;
        5'd10: sine_lut = 246;
        5'd11: sine_lut = 234;
        5'd12: sine_lut = 219;
        5'd13: sine_lut = 199;
        5'd14: sine_lut = 177;
        5'd15: sine_lut = 153;
        5'd16: sine_lut = 128;
        5'd17: sine_lut = 103;
        5'd18: sine_lut = 79;
        5'd19: sine_lut = 57;
        5'd20: sine_lut = 37;
        5'd21: sine_lut = 22;
        5'd22: sine_lut = 10;
        5'd23: sine_lut = 3;
        5'd24: sine_lut = 0;
        5'd25: sine_lut = 3;
        5'd26: sine_lut = 10;
        5'd27: sine_lut = 22;
        5'd28: sine_lut = 37;
        5'd29: sine_lut = 57;
        5'd30: sine_lut = 79;
        5'd31: sine_lut = 103;
        default: sine_lut = 128;
    endcase
end

endmodule
