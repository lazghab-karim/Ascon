from flask import Flask, render_template, redirect, url_for, session, jsonify, request
import requests
import os
from ascon import encrypt

app = Flask(__name__)
app.secret_key = "change_this_secret_key"



ASCON_KEY = b"mysecretkey12345" 
ASCON_NONCE = b"unique_nonce_123"  

ESP32_IP = "192.168.1.114"
ESP32_BASE = f"http://{ESP32_IP}"

esp32 = requests.Session()



@app.route("/")
def dashboard():
    if not session.get("esp32_auth"):
        return redirect(url_for("login"))
    return render_template("login.html")



@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "GET":
        return render_template("login.html")

    password = request.form.get("password")

    if password == "adminadmin":
        session["esp32_auth"] = True
        return render_template("dashboard.html")

    return render_template("login.html", error="ESP32 login failed")



@app.route("/logout")
def logout():
    esp32.get(f"{ESP32_BASE}/logout", timeout=5)
    session.clear()
    return redirect(url_for("login"))



@app.route("/led", methods=["POST"])
def led_post():
    if not session.get("esp32_auth"):
        return redirect(url_for("login"))

    state = request.form.get("state", "").lower()
    if state not in ("on", "off"):
        return "Invalid", 400

    
    plaintext = state.encode('utf-8')
    associated_data = b"led_control"  
    ciphertext = encrypt(ASCON_KEY, ASCON_NONCE, associated_data, plaintext)
    
    try:
        r = esp32.post(
            f"{ESP32_BASE}/led",
            data=ciphertext,
            headers={"Content-Length":str(len(ciphertext)),"Content-Type": "text/plain"},
            timeout=5
        )
        return r.text
    except Exception as e:
        return f"ESP32 communication error: {str(e)}", 500

@app.route("/value")
def value():
    if not session.get("esp32_auth"):
        return jsonify({"error": "Unauthorized"}), 401

    r = esp32.get(f"{ESP32_BASE}/value", timeout=5)
    return jsonify({"value": r.text})


if __name__ == "__main__":
    app.run(debug=True)
