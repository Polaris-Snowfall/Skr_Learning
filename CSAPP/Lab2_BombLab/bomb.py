from pwn import *
import string
context.log_level = 'debug'
context.arch = 'amd64'
context.terminal = ['tmux', 'splitw', '-h', '-F' '#{pane_pid}', '-P']

# 启动程序
binary = 'bomb'
# remo = ''
# m = remo.index(':')
# url = remo[:m]
# port = remo[m+1:]
# io = remote(url,port)
io = process('./'+binary)
# io = gdb.debug('./'+binary,'''
# b *0x4011ab
# c

# ''')
# e = ELF('./'+binary)
# libc = ELF('/home/znl/pwn/glibc/')

def phase_5(goal):
    stringarray = [ch for ch in string.ascii_lowercase + string.ascii_uppercase + string.digits]
    for i in goal:
        for test in stringarray:
            if ord(test)&0xf == i:
                print(test)
                break 

phase_5([9,15,14,5,6,7])
io.sendlineafter('which to blow yourself up. Have a nice day!','Border relations with Canada have never been better.')
io.sendlineafter('Phase 1 defused. How about the next one?','1 2 4 8 16 32')
io.sendlineafter("That's number 2.  Keep going!",'1 311')
io.sendlineafter('Halfway there!','7 0 DrEvil')
io.sendlineafter('So you got that one.  Try this one.','ionefg')
io.sendlineafter('Good work!  On to the next...','4 3 2 1 6 5')
io.sendlineafter('But finding it and solving it are quite different...','22')
# gdb.attach(io,'''
# 
# ''')

io.interactive()