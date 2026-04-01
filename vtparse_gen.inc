class VTParser:
    """
    this is the parser class; see the c src at

        https://github.com/haberman/vtparse

    for details. a parser instance has a single
    public method

        __call__(text-to-parse)

    the callback is called as needed as follows:

        callback(action, ch)

    where action is one of the VTPARSE_ACTION_XXX
    constants.
    """

    MAX_INTERMEDIATE_CHARS = 2

    def __init__(self, callback):
        self.cb = callback
        self.state = VTPARSE_STATE_GROUND
        self.intermediate_chars = []
        self.num_intermediate_chars = 0
        self.params = []
        self.num_params = 0
        self.ignore_flagged = False

    def do_action(self, action, ch):
        "perform the specified action for the char ch"
        if action in (
            VTPARSE_ACTION_PRINT,
            VTPARSE_ACTION_EXECUTE,
            VTPARSE_ACTION_HOOK,
            VTPARSE_ACTION_PUT,
            VTPARSE_ACTION_OSC_START,
            VTPARSE_ACTION_OSC_PUT,
            VTPARSE_ACTION_OSC_END,
            VTPARSE_ACTION_UNHOOK,
            VTPARSE_ACTION_CSI_DISPATCH,
            VTPARSE_ACTION_ESC_DISPATCH,
        ):
            self.cb(action, ch)
        elif action == VTPARSE_ACTION_IGNORE:
            pass
        elif action == VTPARSE_ACTION_COLLECT:
            if self.num_intermediate_chars == self.MAX_INTERMEDIATE_CHARS:
                self.ignore_flagged = True
            else:
                self.intermediate_chars.append(ch)
                self.num_intermediate_chars += 1
        elif action == VTPARSE_ACTION_PARAM:
            if ch == ";":
                self.params.append(0)
                self.num_params += 1
            else:
                if not self.num_params:
                    self.num_params = 1
                    self.params.append(0)
                current = self.num_params - 1
                self.params[current] *= 10
                self.params[current] += ch - ord("0")
        elif action == VTPARSE_ACTION_CLEAR:
            self.num_intermediate_chars = 0
            del self.intermediate_chars[:]
            self.num_params = 0
            del self.params[:]
            self.ignore_flagged = False
        else:
            self.cb(VTPARSE_ACTION_ERROR, 0)

    def ACTION(self, state_change):
        "extract action from state change"
        return state_change & 0x0F

    def STATE(self, state_change):
        "extract state from state change"
        return state_change >> 4

    def do_state_change(self, change, ch):
        "perform a state change"
        new_state = self.STATE(change)
        action = self.ACTION(change)

        if new_state:
            exit_action = EXIT_ACTIONS[self.state - 1]
            entry_action = ENTRY_ACTIONS[self.state - 1]
            if exit_action:
                self.do_action(exit_action, 0)

            if action:
                self.do_action(action, ch)

            if entry_action:
                self.do_action(entry_action, 0)

            self.state = new_state
        else:
            self.do_action(action, ch)

    def __call__(self, buffer):
        if isinstance(buffer, str):
            buffer = buffer.encode(locale.getpreferredencoding())
        for byte in buffer:
            b = ord(byte) if isinstance(byte, str) else byte
            change = STATE_TABLE[self.state - 1][b]
            self.do_state_change(change, b)


def clean(text):
    "strip terminal control codes from text"
    ret = []

    def cb(action, ch):
        if action in (VTPARSE_ACTION_EXECUTE, VTPARSE_ACTION_PRINT):
            ret.append(ch)

    VTParser(cb)(text)
    try:
        _ = unicode
        return "".join(chr(x) for x in ret).decode(ENCODING)
    except NameError:
        return bytes(ret).decode(ENCODING)


def main():
    "main program: clean stdin"
    print(clean(sys.stdin.read()), end="")


if __name__ == "__main__":
    main()


## vim: ft=python
