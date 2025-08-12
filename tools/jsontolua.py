#! /usr/bin/python3
# https://github.com/neoliang/jsontolua
# With slight changes to run as cli and wrap in return.
import json
import sys
import argparse
import os

LUA_KEYWORDS = {
    "and", "break", "do", "else", "elseif", "end", "false", "for",
    "function", "goto", "if", "in", "local", "nil", "not", "or",
    "repeat", "return", "then", "true", "until", "while"
}

def is_identifier(s):
    """Return True if s is a valid Lua identifier and not a reserved keyword."""
    return (
        isinstance(s, str)
        and s.isidentifier()
        and not s.isnumeric()
        and s not in LUA_KEYWORDS
    )

def space_str(layer):
    return '\t' * layer

def escape_string(s):
    return "'" + s.replace("\\", "\\\\").replace("'", "\\'") + "'"

def dic_to_lua_str(data, layer=0):
    if isinstance(data, str):
        yield escape_string(data)
    elif isinstance(data, bool):
        yield 'true' if data else 'false'
    elif isinstance(data, (int, float)):
        yield str(data)
    elif isinstance(data, list):
        yield "{\n"
        for i, item in enumerate(data):
            yield space_str(layer + 1)
            yield from dic_to_lua_str(item, layer + 1)
            if i < len(data) - 1:
                yield ",\n"
        yield "\n" + space_str(layer) + "}"
    elif isinstance(data, dict):
        yield "{\n"
        items = list(data.items())
        for i, (k, v) in enumerate(items):
            yield space_str(layer + 1)
            if isinstance(k, int):
                yield f"[{k}] = "
            elif is_identifier(k):
                yield f"{k} = "
            else:
                yield f"[{escape_string(k)}] = "
            yield from dic_to_lua_str(v, layer + 1)
            if i < len(items) - 1:
                yield ",\n"
        yield "\n" + space_str(layer) + "}"
    else:
        raise TypeError(f"Unsupported type: {type(data)}")

def str_to_lua_table(json_str, return_wrapper=True):
    data = json.loads(json_str)
    out = []
    if return_wrapper:
        out.append("return ")
    out.extend(dic_to_lua_str(data))
    out.append("\n")
    return ''.join(out)

def file_to_lua_file(json_file, lua_file, return_wrapper=True):
    with open(json_file, 'r', encoding='utf-8') as jf:
        json_str = jf.read()
        lua_str = str_to_lua_table(json_str, return_wrapper)
    with open(lua_file, 'w', encoding='utf-8') as lf:
        lf.write(lua_str)

def process_directory(input_dir, return_wrapper=True):
    for root, _, files in os.walk(input_dir):
        for name in files:
            if name.endswith(".json"):
                json_path = os.path.join(root, name)
                lua_path = os.path.splitext(json_path)[0] + ".lua"
                try:
                    file_to_lua_file(json_path, lua_path, return_wrapper)
                    print(f"✅ {json_path} → {lua_path}")
                except Exception as e:
                    print(f"❌ Failed to convert {json_path}: {e}")

def main():
    parser = argparse.ArgumentParser(description='Convert JSON to Lua table(s)')
    parser.add_argument('input', nargs='?', help='Input JSON file (or ignored if --dir is used)')
    parser.add_argument('output', nargs='?', help='Output Lua file (only used in single-file mode)')
    parser.add_argument('--no-return', action='store_true', help='Don\'t wrap in "return {...}"')
    parser.add_argument('--dir', help='Process all JSON files in this directory (recursively)')
    args = parser.parse_args()

    return_wrapper = not args.no_return

    if args.dir:
        if not os.path.isdir(args.dir):
            print(f"❌ Directory does not exist: {args.dir}")
            sys.exit(1)
        process_directory(args.dir, return_wrapper)
    elif args.input and args.output:
        file_to_lua_file(args.input, args.output, return_wrapper)
        print(f"✅ {args.input} → {args.output}")
    else:
        parser.print_help()
        sys.exit(1)

if __name__ == '__main__':
    main()
