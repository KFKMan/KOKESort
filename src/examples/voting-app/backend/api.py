import json
import os
import KOKESortWrapper
import copy

class Candidate:
    def __init__(self, name: str, vote_count: int):
        self.name = name
        self.vote_count = vote_count

    def to_dict(self):
        return {
            "name": self.name,
            "voteCount": self.vote_count
        }

    def __repr__(self):
        return f"{self.name} ({self.vote_count})"

class API:
    def __init__(self):
        self.voters_file = "backend/data/voters.json"
        self.state_file = "backend/data/state.json"

        if not os.path.exists(self.voters_file):
            print("Voters file not found")

    def _read_json(self, path):
        if not os.path.exists(path):
            return [] if path.endswith("voters.json") else {}
        with open(path, "r", encoding="utf-8") as f:
            return json.load(f)

    def _write_json(self, path, data):
        with open(path, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=2, ensure_ascii=False)

    def has_voted(self, tc):
        voters = self._read_json(self.voters_file)
        return any(v["tc"] == tc for v in voters)

    def get_voters(self):
        return self._read_json(self.voters_file)

    def get_province_code(self, province_name):
        state = self._read_json(self.state_file)
        for province in state.get("country", []):
            if province["name"].strip().lower() == province_name.strip().lower():
                return province["code"]
        return None


    def get_candidates(self, province):
        province_code = province

        state = self._read_json(self.state_file)
        candidates = {
            "president": {},
            "mayor": {}
        }

        if not province_code.isnumeric():
            province_code = self.get_province_code(province_code)
            print("Province code:", province_code)

        if province_code is None:
            return {"status": "error", "message": "Geçersiz il."}

        for pres in state.get("presidents", []):
            candidates["president"][f"{pres['name']} {pres['surname']}"] = pres["voteCount"]

        for mayor in state.get("mayors", []):
            if mayor["targetCountry"] == province_code:
                candidates["mayor"][f"{mayor['name']} {mayor['surname']}"] = mayor["voteCount"]

        pres_list = [{"name": k, "voteCount": int(v)} for k, v in candidates["president"].items()]

        # Sorting here

        candidates["president"] = {f"{item['name']}": item["voteCount"] for item in pres_list}

        mayor_list = [{"name": k, "voteCount": int(v)} for k, v in candidates["mayor"].items()]

        # Sorting here

        candidates["mayor"] = {f"{item['name']}": item["voteCount"] for item in mayor_list}

        return candidates

    def add_candidate(self, province, role, name):
        state = self._read_json(self.state_file)
        if province not in state:
            state[province] = {"president": {}, "mayor": {}}
        if name in state[province][role]:
            return {"status": "error", "message": "Aday zaten mevcut."}
        state[province][role][name] = 0
        self._write_json(self.state_file, state)
        return {"status": "ok"}

    def submit_vote(self, name, surname, tc, province, president, mayor):
        if not tc.isdigit() or len(tc) != 11:
            return {"status": "error", "message": "Geçersiz TC Kimlik No."}

        if self.has_voted(tc):
            return {"status": "error", "message": "Bu TC ile daha önce oy kullanılmış."}

        # Voters dosyasını güncelliyoruz
        voters = self._read_json(self.voters_file)
        voters.append({"name": name, "surname": surname, "tc": tc, "province": province})
        self._write_json(self.voters_file, voters)

        # State dosyasını okuyoruz
        state = self._read_json(self.state_file)

        # İlgili ilde adayların geçerli olup olmadığını kontrol ediyoruz
        province_data = next((p for p in state["country"] if p["code"] == province), None)
        if not province_data:
            return {"status": "error", "message": "Geçersiz il."}

        # Cumhurbaşkanı ve Belediye Başkanının geçerli olup olmadığını kontrol ediyoruz
        president_data = next((p for p in state["presidents"] if f"{p['name']} {p['surname']}" == president), None)
        mayor_data = next((m for m in state["mayors"] if f"{m['name']} {m['surname']}" == mayor and m["targetCountry"] == province_data["code"]), None)

        if not president_data or not mayor_data:
            return {"status": "error", "message": "Geçersiz aday seçimi."}

        # Oy sayısını artırıyoruz
        president_data["voteCount"] = str(int(president_data["voteCount"]) + 1)
        mayor_data["voteCount"] = str(int(mayor_data["voteCount"]) + 1)

        # Güncellenmiş state dosyasını yazıyoruz
        self._write_json(self.state_file, state)

        return {"status": "ok"}


    # İl listesini almak için fonksiyon
    def get_countries(self):
        state = self._read_json(self.state_file)
        countries = [{"name": country["name"], "code": country["code"]} for country in state.get("country", [])]
        return countries
