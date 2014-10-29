import info_status
import stest
import sample_frame_module_common

# Verify that info/status commands have been registered for all
# classes in this module.
info_status.check_for_info_status(['sample-frame-module'])

# Create an instance of each object defined in this module
dev = sample_frame_module_common.create_sample_frame_module()

# Run info and status on each object. It is difficult to test whether
# the output is informative, so we just check that the commands
# complete nicely.
for obj in [dev]:
    for cmd in ['info', 'status']:
        try:
            SIM_run_command(obj.name + '.' + cmd)
        except SimExc_General, e:
            stest.fail(cmd + ' command failed: ' + str(e))