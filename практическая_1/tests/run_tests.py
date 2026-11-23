"""Характеризационные тесты (golden master) для программы управления теплицей.

Запуск:
    python tests/run_tests.py           # собрать src/greenhouse.cpp и сверить с эталоном
    python tests/run_tests.py --legacy  # то же самое для legacy/greenhouse_legacy.cpp
    python tests/run_tests.py --record  # пересоздать эталон по legacy-версии

Эталон (tests/cases/*.out и *.code) записывается ТОЛЬКО по legacy-версии:
это и есть фиксация поведения, которое рефакторинг обязан сохранить.
"""

import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CASES_DIR = ROOT / "tests" / "cases"
LEGACY_SRC = ROOT / "legacy" / "greenhouse_legacy.cpp"
CURRENT_SRC = ROOT / "src" / "greenhouse.cpp"

# Каждый сценарий: имя -> строки, которые «вводит пользователь».
CASES = {
    # --- ввод влажности ---
    "moisture_not_number": "abc\n",
    "moisture_empty_input": "",
    "moisture_negative": "-1\n",
    "moisture_above_100": "101\n",
    "moisture_low_bound_0": "0\n0\n",
    "moisture_high_bound_100": "100\n0\n",
    # --- выход и ошибки ввода действия ---
    "exit_immediately": "50\n0\n",
    "action_not_number": "50\nx\n",
    "action_eof_without_exit": "50\n1\n",
    "unknown_actions_do_not_count": "50\n9\n-1\n5\n0\n",
    # --- 1: статус ---
    "status": "50\n1\n0\n",
    # --- 2: открыть клапан ---
    "open_ok": "30\n2\n0\n",
    "open_ok_at_59": "59\n2\n0\n",
    "open_denied_at_60": "60\n2\n0\n",
    "open_denied_already_open": "30\n2\n2\n0\n",
    # --- 3: закрыть клапан ---
    "close_after_open": "30\n2\n3\n0\n",
    "close_when_already_closed_counts": "30\n3\n3\n0\n",
    # --- 4: импульс полива ---
    "pulse_ok": "30\n4\n0\n",
    "pulse_ok_at_84_clamped_to_100": "84\n4\n0\n",
    "pulse_denied_at_85": "85\n4\n0\n",
    "pulse_denied_valve_open": "30\n2\n4\n0\n",
    "pulse_twice_then_open_denied": "20\n4\n4\n2\n0\n",
    "pulse_until_denied": "10\n4\n4\n4\n4\n4\n0\n",
    # --- лимит в 8 операций ---
    "limit_8_status": "50\n1\n1\n1\n1\n1\n1\n1\n1\n1\n",
    "limit_8_denied_not_counted": "70\n2\n2\n1\n1\n1\n1\n1\n1\n1\n1\n",
    "limit_8_mixed": "10\n4\n2\n4\n3\n4\n1\n3\n2\n1\n7\n",
    # --- длинный смешанный сценарий ---
    "mixed_scenario": "40\n1\n2\n2\n4\n3\n4\n8\n2\n1\n0\n",
}


def find_compiler() -> str:
    for candidate in ("g++", r"C:\msys64\ucrt64\bin\g++.exe"):
        path = shutil.which(candidate) or (candidate if Path(candidate).exists() else None)
        if path:
            return path
    sys.exit("Не найден компилятор g++")


def build(source: Path, out_dir: Path) -> Path:
    exe = out_dir / (source.stem + ".exe")
    cmd = [find_compiler(), "-std=c++17", "-Wall", "-Wextra", "-Werror",
           "-static", "-O2", str(source), "-o", str(exe)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        sys.exit(f"Ошибка сборки {source.name}:\n{result.stderr}")
    return exe


def run(exe: Path, stdin: str) -> tuple[str, int]:
    result = subprocess.run([str(exe)], input=stdin.encode(), capture_output=True, timeout=10)
    return result.stdout.decode().replace("\r\n", "\n"), result.returncode


def main() -> None:
    record = "--record" in sys.argv
    source = LEGACY_SRC if (record or "--legacy" in sys.argv) else CURRENT_SRC
    if "--src" in sys.argv:
        source = Path(sys.argv[sys.argv.index("--src") + 1]).resolve()

    with tempfile.TemporaryDirectory() as tmp:
        exe = build(source, Path(tmp))

        if record:
            CASES_DIR.mkdir(parents=True, exist_ok=True)
            for name, stdin in CASES.items():
                out, code = run(exe, stdin)
                (CASES_DIR / f"{name}.in").write_text(stdin, encoding="utf-8", newline="\n")
                (CASES_DIR / f"{name}.out").write_text(out, encoding="utf-8", newline="\n")
                (CASES_DIR / f"{name}.code").write_text(f"{code}\n", encoding="utf-8", newline="\n")
            print(f"Эталон записан: {len(CASES)} сценариев")
            return

        failed = 0
        for name in CASES:
            stdin = (CASES_DIR / f"{name}.in").read_text(encoding="utf-8")
            want_out = (CASES_DIR / f"{name}.out").read_text(encoding="utf-8")
            want_code = int((CASES_DIR / f"{name}.code").read_text(encoding="utf-8"))
            out, code = run(exe, stdin)
            if out == want_out and code == want_code:
                print(f"  ok    {name}")
            else:
                failed += 1
                print(f"  FAIL  {name}: код {code} (ожидался {want_code})")
                if out != want_out:
                    print("        --- ожидалось ---\n" + want_out + "        --- получено ---\n" + out)

        total = len(CASES)
        print(f"\n{source.name}: пройдено {total - failed} из {total}")
        sys.exit(1 if failed else 0)


if __name__ == "__main__":
    os.chdir(ROOT)
    main()
