
#include <cstdio>
#include <map>
#include <sol/sol.hpp>
#include <string>


static const char* to_string = R"(
function table_print (tt, indent, done)
  print("here")
  done = done or {}
  indent = indent or 0
  if type(tt) == "table" then
    local sb = {}
    for key, value in pairs (tt) do
      table.insert(sb, string.rep (" ", indent)) -- indent it
      if type (value) == "table" and not done [value] then
        done [value] = true
        table.insert(sb, key .. " = {\n");
        table.insert(sb, table_print (value, indent + 2, done))
        table.insert(sb, string.rep (" ", indent)) -- indent it
        table.insert(sb, "}\n");
      elseif "number" == type(key) then
        table.insert(sb, string.format("\"%s\"\n", tostring(value)))
      else
        table.insert(sb, string.format(
            "%s = \"%s\"\n", tostring (key), tostring(value)))
       end
    end
    return table.concat(sb)
  else
    return tt .. "\n"
  end
end

function to_string( tbl )
    if  "nil"       == type( tbl ) then
        return tostring(nil)
    elseif  "table" == type( tbl ) then
        return table_print(tbl)
    elseif  "string" == type( tbl ) then
        return tbl
    else
        return tostring(tbl)
    end
end
)";

//extern "C" {
unsigned readReg(int reg);
void writeReg(int reg, unsigned value);
uint8_t read6502(uint16_t address);
void write6502(uint16_t address, uint8_t value);
//}


static sol::state lua;
static std::unordered_map<int, std::function<void(int)>> breakFunctions;

int callBreakFunction(int what)
{
    auto& fn = breakFunctions[what];
	if(fn) {
		fn(what);
		return 1;
	}
	return 0;
}

void init_scripting()
{
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string,
                       sol::lib::math, sol::lib::table, sol::lib::debug);

    lua.script(to_string);

	lua["set_break_fn"] = [&](int what, std::function<void(int)> const& fn) {
		printf("In break %d\n", what);
		breakFunctions[what] = fn;

	};

	lua["read_reg_6502"] = &readReg;
    lua["write_reg_6502"] = &writeReg;
    lua["write_mem_6502"] = &write6502;
    lua["read_mem_6502"] = &read6502;

}

void load_script(const char* fname)
{
	puts("Executing lua");
	if(!lua.do_file(fname)) {
		puts("Failed");
	}

}