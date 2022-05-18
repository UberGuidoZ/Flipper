Work in progress. Needs the [WAV Player application](https://github.com/flipperdevices/flipperzero-firmware/tree/zlo/wav-player/applications/wav_player) compiled into firmware. Idea and files provided by [RogueMaster](https://github.com/RogueMaster).

Create a wav_player folder in the root of your SD card. You'll need 8-bit unsigned WAV files (try [Audacity](https://www.audacityteam.org/) for conversion.)

WAV Player is apparently very particular about the format. WavPlayer wants BitExact, NO headers, 48k PCM unsigned 8-bit stereo. Example:

`Input #0, wav, from '.\canyon.wav':`<br>
`  Duration: 00:02:03.38, bitrate: 768 kb/s`<br>
`    Stream #0:0: Audio: pcm_u8 ([1][0][0][0] / 0x0001), 48000 Hz, stereo, u8, 768 kb/s`

To produce a compatible file, you can use:<br>
`ffmpeg -i .\input.mp3 -c:a pcm_u8 -fflags +bitexact -flags:a +bitexact -ac 2 -ar 48k output.wav`
