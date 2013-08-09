IMPORTANT: Please, be careful when using these scripts! They are unpolished at
the moment and you'll have to respect a simple rule: call these scripts from
where they are. So, in that case, head yourself to tools/android, then call
./make_all.sh.

HOW-TO-USE:
1) Some of these scripts need an environement variable to work: $NDK
export NDK=/path/to/your/ndk

2) You'll need to make them executable by your system, so:
chmod a+x *.sh

3) Type: ./make_all.sh which should create your toolchains, download the
external libraries and compile them.

These scripts will be improved over time. Meanwhile, you'll have to play with
them if you want a customied behavior.

Good luck!

PS: OpenAL-Soft is missing at the moment (because it's coming with other
changes) but it will be very available very soon.
