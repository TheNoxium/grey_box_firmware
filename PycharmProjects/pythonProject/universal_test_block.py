from sanic import Sanic
from sanic import Blueprint
from sanic import json
import time

bp = Blueprint('routes', url_prefix='/api')

last_id = 0
profiles = dict()
current_status = False


@bp.post('/add_profile')
async def add_profile(request):
    global last_id
    data = request.json
    if data:
        if "start_time" in data.keys() and "end_time" in data.keys() and 0 <= int(
                data["start_time"]) < 86400 and 0 < int(data["end_time"]) < 86400 and int(data["start_time"]) < int(
                data["end_time"]):
            print("add_profile: success")
            id_cur = last_id + 1
            last_id += 1
            profiles.update({id_cur: data})
            print(profiles)
        else:
            return json({"error": "data is wrong"}, status=400)
    else:
        return json({"error": "data is missing"}, status=400)

    return json({"message": "add_profile received successfully",
                 "id": id_cur,
                 "start_time": data["start_time"],
                 "end_time": data["end_time"]})


@bp.put('/modify_profile')
async def modify_profile(request):
    data = request.json
    if data:
        if "start_time" in data.keys() and "end_time" in data.keys() and "id" in data.keys() and 0 <= int(
                data["start_time"]) < 86400 and 0 < int(data["end_time"]) < 86400 and int(data["start_time"]) < int(
                data["end_time"]) and data["id"] in profiles.keys():
            print("modify: success")
            id = data.pop("id")
            profiles.update({id: data})
            print(profiles)
        else:
            return json({"error": "data is wrong"}, status=400)
    else:
        return json({"error": "data is missing"}, status=400)

    return json({"message": "modify received successfully",
                 "id": id,
                 "start_time": data["start_time"],
                 "end_time": data["end_time"]})


@bp.delete('/remove_profile')
async def remove_profile(request):
    data = request.json
    if data:
        if "id" in data.keys() and data["id"] in profiles.keys():
            print("remove: success")
            profiles.pop(data["id"])
            print(profiles)
        else:
            return json({"error": "data is wrong"}, status=400)
    else:
        return json({"error": "data is missing"}, status=400)

    return json({"message": "remove received successfully",
                 "id": data["id"]})


@bp.get('/get_profile_list')
async def get_profile_list(request):
    global profiles
    profiles_list = list()
    for profile in profiles.keys():
        local_profile = {"id": profile}
        local_profile.update(profiles[profile])
        profiles_list.append(local_profile)
    return json({"message": "get_profile_list received successfully",
                 "profile_list": profiles_list})


@bp.get('/get_current_status')
async def get_current_status(request):
    global current_status
    return json({"message": "get_current_status received successfully",
                 "current_status": current_status})


@bp.get('/get_counter_data')
async def get_counter_data(request):
    return json({"message": "get_counter_data received successfully",
                 "data": time.time()})


@bp.post('/instant_start')
async def instant_start(request):
    global current_status
    current_status = True
    return json({"message": "instant_start received successfully",
                 "current_status": current_status})


@bp.post('/forced_stop')
async def forced_stop(request):
    global current_status
    current_status = False
    return json({"message": "forced_stop received successfully",
                 "current_status": current_status})


sanic = Sanic(__name__)
sanic.blueprint(bp)

# @sanic.route('/api/add_profile', methods=["POST"])
# async def add_profile(request):
#     data = request.json
#     if data:
#         print('1')
#     else:
#         return json({"error": "data is missing"}, status=400)
#     return json({"message": "add_profile received successfully"})
#
# @sanic.route('/modify_profile', methods=["PUT"])
# async def modify_profile(request):
#     data = request.json
#     if not data:
#         return json({"error": "data is missing"}, status=400)
#     return json({"message": "set_local_mode received successfully"})


if __name__ == '__main__':
    sanic.run(host="0.0.0.0", port=8000)
