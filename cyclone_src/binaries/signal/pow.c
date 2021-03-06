/* Copyright (c) 2002-2003 krzYszcz and others.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#include <math.h>
#include "m_pd.h"

static t_class *pow_class;

typedef struct _pow
{
    t_object x_obj;
    t_inlet  *x_inlet;
} t_pow;

static t_int *pow_perform(t_int *w)
{
    int nblock = (int)(w[1]);
    t_float *in1 = (t_float *)(w[2]);
    t_float *in2 = (t_float *)(w[3]);
    t_float *out = (t_float *)(w[4]);
    while (nblock--)
    {
        t_float f1 = *in1++;
        t_float f2 = *in2++;
       *out++ = powf(f2, f1);
    }
    return (w + 5);
}

static void pow_dsp(t_pow *x, t_signal **sp)
{
    dsp_add(pow_perform, 4, sp[0]->s_n,
            sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec);
}

static void *pow_free(t_pow *x)
{
    inlet_free(x->x_inlet);
    return (void *)x;
}

static void *pow_new(t_floatarg f)
{
    t_pow *x = (t_pow *)pd_new(pow_class);
    x->x_inlet = inlet_new((t_object *)x, (t_pd *)x, &s_signal, &s_signal);
    pd_float((t_pd *)x->x_inlet, f);
    outlet_new((t_object *)x, &s_signal);
    return (x);
}

void pow_tilde_setup(void)
{
    pow_class = class_new(gensym("cyclone/pow~"), (t_newmethod)pow_new,
        (t_method)pow_free, sizeof(t_pow), CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addmethod(pow_class, nullfn, gensym("signal"), 0);
    class_addmethod(pow_class, (t_method)pow_dsp, gensym("dsp"), A_CANT, 0);
}