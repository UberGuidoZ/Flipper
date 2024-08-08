// By jamisonderek
// For documentation on this API's use, see:
// https://github.com/jamisonderek/flipper-zero-tutorials/blob/main/js/
({
    _acquired: false,
    _acquire: ffi("int furi_hal_speaker_acquire(int)"),
    _release: ffi("void furi_hal_speaker_release(void)"),
    start: ffi("void furi_hal_speaker_start(float, float)"),
    stop: ffi("void furi_hal_speaker_stop(void)"),
    acquire: function (timeout) {
        if (!this._acquired) {
            this._acquired = this._acquire(timeout);
        }
        return this._acquired;
    },
    acquired: function () {
        return this._acquired;
    },
    release: function () {
        if (this._acquired) {
            this._release();
            this._acquired = false;
        }
    },
    play: function (frequency, volume, duration) {
        let already_acquired = this.acquired();
        if (!already_acquired) {
            this.acquire(1000);
        };
        if (this.acquired()) {
            this.start(frequency, volume);
            delay(duration);
            this.stop();
        }
        if (!already_acquired) {
            this.release();
        }
    },
}
)