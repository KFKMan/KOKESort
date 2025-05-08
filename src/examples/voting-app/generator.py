import json
import random
import csv

def load_names(file_path):
    with open(file_path, encoding='utf-8') as f:
        return [line.strip() for line in f if line.strip()]
    
def load_names_from_csv(file_path):
    names = []
    with open(file_path, encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            try:
                frequencyStr = row["Frequency"].strip()
                if not frequencyStr.isdigit():
                    continue
                frequency = int(frequencyStr)
                if frequency > 5000:
                    print(f"High frequency: {frequency}")
                    names.append(row["Name"].capitalize())
            except (ValueError, KeyError):
                continue
    return names

first_names = load_names_from_csv("names.csv")
last_names = load_names_from_csv("surnames.csv")

def generate_mayors(first_names, last_names, candidates_per_city=4):
    mayors = []
    for cityCode in range(1, 82):
        for _ in range(candidates_per_city):
            name = random.choice(first_names)
            surname = random.choice(last_names)
            mayors.append({
                "name": name,
                "surname": surname,
                "targetCountry": str(cityCode),
                "voteCount": str(0)
            })
    return mayors

mayors = generate_mayors(first_names, last_names)

# JSON olarak kaydet (isteğe bağlı)
with open("mayors.json", "w", encoding="utf-8") as f:
    json.dump(mayors, f, ensure_ascii=False, indent=2)
