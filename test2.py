import json

with open("/home/losangelous/.local/share/opencode/tool-output/tool_d4269b561001ppahWi95PdgfDZ", "r") as f:
    text = f.read()

# find the last GOT BODY
body_start = text.rfind("GOT BODY: {")
if body_start != -1:
    body = text[body_start + 10:]
    try:
        data = json.loads(body)
        res = data.get("result")
        print(f"Type of result: {type(res)}")
        if isinstance(res, list):
            print(f"Number of items in list: {len(res)}")
            print("First item:", json.dumps(res[0], indent=2))
        elif isinstance(res, dict):
            print("Keys in dict:", res.keys())
            if "items" in res:
                items = res["items"]
                print(f"Number of items in items: {len(items)}")
                print("First item:", json.dumps(items[0], indent=2))
    except Exception as e:
        print("Error:", e)
