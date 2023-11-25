import React from 'react'
import PropTypes from 'prop-types'
import reactCSS from 'reactcss'
import merge from 'lodash/merge'

import { ColorWrap } from '../common/ColorWrap'
import { Lightness }from '../common/Lightness'

import LightnessPointer from './LightnessPointer'

export const LightnessPicker = ({ width, height, onChange, hsl, direction, pointer,
  styles: passedStyles = {}, className = '' }) => {
  const styles = reactCSS(merge({
    'default': {
      picker: {
        position: 'relative',
        width,
        height,
      },
      lightness: {
        radius: '2px',
      },
    },
  }, passedStyles))

  // Overwrite to provide pure lightness color
  const handleChange = data => onChange({ a: 1, h: data.h, l: data.l, s:data.s })

  return (
    <div style={ styles.picker } className={ `lightness-picker ${ className }` }>
      <Lightness
        { ...styles.lightness }
        hsl={ hsl }
        pointer={ pointer }
        onChange={ handleChange }
        direction={ direction }
      />
    </div>
  )
}

LightnessPicker.propTypes = {
  styles: PropTypes.object,
}
LightnessPicker.defaultProps = {
  width: '316px',
  height: '16px',
  direction: 'horizontal',
  pointer: LightnessPointer,
  styles: {},
}

export default ColorWrap(LightnessPicker)
