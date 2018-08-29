      	ChainLink, version 1.3.0
      	Last update: 17 Oct 2007
      	Type 'demo' to get started.
>> cd /home/vbbproc/users/michael/fea/linear_code/fesbi_linear/v2
      Current directory is /home/vbbproc/users/michael/fea/linear_code/fesbi_linear/v2
>> x=readmda('box1.Z0.force.mda')
      x = mda (real) 100x102x121x3
>> viewmda(x)
      ans = (viewmdaPtr)
>> fz=x(:,:,:,3)
      fz = mda (real) 100x102x121x1
>> fz=squuze(x(:,:,:,3))
      Error in execution: Error: no match for: squuze(mda)
>> fz=squeeze(x(:,:,:,3))
      fz = mda (real) 100x102x121
>> viewmda(fz)
      ans = (viewmdaPtr)
>> viewmda(x)

