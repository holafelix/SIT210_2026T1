import asyncio
import json
import queue
import sounddevice as sd
from vosk import Model, KaldiRecognizer
from bleak import BleakScanner, BleakClient

DEVICE_NAME = "NanoLights"                                
CMD_CHAR_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214"      
SAMPLE_RATE = 16000

audio_q = queue.Queue()

def audio_callback(indata, frames, time, status):
    audio_q.put(bytes(indata))

def phrase_to_command(text):
    text = text.lower()
    if "lights on" in text:
        return "lights_on"
    if "lights off" in text:
        return "lights_off"
    if "fan on" in text:
        return "fan_on"
    if "fan off" in text:
        return "fan_off"
    return None

async def find_device():
    devices = await BleakScanner.discover(timeout=5.0)
    for d in devices:
        if d.name == DEVICE_NAME:
            return d
    return None

async def main():
    model = Model(MODEL_PATH)
    grammar = '["lights on", "lights off", "fan on", "fan off", "[unk]"]'
    rec = KaldiRecognizer(model, SAMPLE_RATE, grammar)

    device = await find_device()
    if device is None:
        print("Could not find NanoLights. Is the Arduino powered and advertising?")
        return

    async with BleakClient(device) as client:
        print("Connected to", device.name)
        with sd.RawInputStream(samplerate=SAMPLE_RATE, blocksize=8000,
                               dtype="int16", channels=1, callback=audio_callback):
            print("Listening... say: lights on / lights off / fan on / fan off")
            loop = asyncio.get_event_loop()
            while True:
                data = await loop.run_in_executor(None, audio_q.get)
                if rec.AcceptWaveform(data):
                    text = json.loads(rec.Result()).get("text", "")
                    if text:
                        print("Heard:", text)
                        command = phrase_to_command(text)
                        if command:
                            await client.write_gatt_char(CMD_CHAR_UUID, command.encode())
                            print("Sent:", command)

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nStopped.")
