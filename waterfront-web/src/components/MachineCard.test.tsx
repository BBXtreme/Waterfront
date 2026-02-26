import { render, screen } from '@testing-library/react'
import { expect, test } from 'vitest'
import MachineCard from './MachineCard'

test('renders connected status in green', () => {
  render(<MachineCard status="connected" />)
  const statusElement = screen.getByText(/connected/i)
  expect(statusElement).toBeInTheDocument()
  // Match the actual classes used in MachineCard.tsx
  expect(statusElement).toHaveClass('text-green-700')
  expect(statusElement).toHaveClass('bg-green-50')
})