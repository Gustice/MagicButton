# Button Shell

The device support primarily an interactively usable shell that utilizes human readable commands.
It also can be set to some kind of automatic mode (either ascii or binary) in order to simplify automated access.
The versatility of this interface is due to proof of concept for further projects.

**NOTE:** Dtr-Signal must to be set in order to be able to tranceive messages from/to the device.

## Interactive Shell interface

Each line represents an event, or a command. The commands are separated in following categories:
- The Leading `>`-sign is printed in terminal only and indicates following user input or commands.
- The Leading `<`-sign indicates responses to commands after successful processing
- The Leading `!`-sign indicates error responses to commands or input
- The Leading `:`-sign indicates an acyclic event 

The user gets an welcome message displays that also displays how the get the help output. 

### Supported commands

- Leading `*...` suppresses echo for current line

- `?` Displays help text

- `#...` Shortcut command
  Accepted Shortcuts are
  - `#r`, `#g`, `#b`, `#c`, `#m`, `#y`, `#w`, for colors
  - `#0`, `#1`, `#2`, `#3`, for modes

- `Get Version:` Prints version in form `Device Version` (e.g. "MagicButton V0.5")

- `Set Color:RR,GG,BB,WW` to set color in idle mode (no pulsing or breathing effects)
  Values are to be given comma separated in hex-mode

- `Set Effect:...` 
  Accepted Values are
  - `Idle`: Intended to signal idle state 
  - `Processing`: Intended for busy-signal 
  - `Good`: Intended for successful-signal
  - `Bad`: Intended for failed-signal

- `Get Status:` Returns status of button. 
  Prints status in form `State:Btn=...;Effect=...;` 

  where button state is:
  - `Idle`: Button not pressed
  - `Pressed`: Transition state from idle to pressed
  - `Holding`: Button is pressed and hold
  - `Released`: Transition state from pressed to idle

  and Effect state is: 
  - `Idle`: Intended to signal idle state 
  - `Processing`: Intended for busy-signal 
  - `Good`: Intended for successful-signal
  - `Bad`: Intended for failed-signal

- `Set Echo:...`: Disables echo (for automated mode)
  Accepted SW-Parameter are
  - `on`: enables echo;
  - `disables`: disables echo;

- Button event: The device issues a message in form `:Btn:L->H`, on button press and `:Btn:H->L` on button release.


### Example

Following output shows an example session

``` Text 
... Connecting to COM4 --- forcing DTR active
         Connected!
** Magic Button Shell **
* Get Help by type ?-Symbol
$ > ?** Magic Button Shell HELP **
* String-Commands: 
* 'Get Version:' -> Prints version
  ...

$ > Get Version:
<MagicButton V0.5
$ > Set Effect:Processing
<Setting Effect
$ > Get Status:
<State:Btn=Idle;Effect=Processing;
$ > asdf
!Unknown command. See help with '?' ...
MB >
```

## Automated Ascii-API

## Automated Binary-API

