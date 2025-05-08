import webview
from backend.api import API

if __name__ == '__main__':
    api = API()
    window = webview.create_window("Voting App", "index.html", js_api=api, width=600, height=700)
    webview.start(debug=True)