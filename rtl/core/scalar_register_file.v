// 
// Copyright 2011-2012 Jeff Bush
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 

//
// Storage for scalar registers, 2 read ports and 1 write port.
//

module scalar_register_file(
	input 					clk,
	input [6:0] 			ds_scalar_sel1,
	input [6:0] 			ds_scalar_sel2,
	output reg[31:0] 		scalar_value1,
	output reg[31:0] 		scalar_value2,
	input [6:0] 			wb_writeback_reg,
	input [31:0] 			wb_writeback_value,
	input 					enable_scalar_reg_store);

	localparam NUM_REGISTERS = 4 * 32; // 32 registers per strand * 4 strands

	reg[31:0]				registers[0:NUM_REGISTERS - 1];	
	integer					i;
	
	initial
	begin
		for (i = 0; i < NUM_REGISTERS; i = i + 1)
			registers[i] = 0;
	end
	
	always @(posedge clk)
	begin
		scalar_value1 <= #1 registers[ds_scalar_sel1];
		scalar_value2 <= #1 registers[ds_scalar_sel2];
		if (enable_scalar_reg_store)
			registers[wb_writeback_reg] <= #1 wb_writeback_value;
	end
	
endmodule
